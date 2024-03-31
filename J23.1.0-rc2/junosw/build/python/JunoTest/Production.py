import logging
import os, sys
import copy
import types
import argparse
from .Job import *
from . import Job
import re

# compatible between 2 and 3
try:
  import configparser
except ImportError:
  import ConfigParser as configparser


class Production:
  '''
      Configuration priority: cmd line > step local > production overall
  '''

  def __init__(self):
    self.overallCFG = ProductionConfig()
    self.stepCFG = []
    self.cmdCFG = {}
    self.steps = []
    self.user_workflows = [] # specify in ini file, user could select which to use.
    self.logFile = self.overallCFG.getAttr('log')
    self._configureArg()
    user_extra_dir = []
    user_extra_dir_str = os.getenv("JUNOTESTDRIVERPATH")
    if user_extra_dir_str:
      user_extra_dir.extend(user_extra_dir_str.split(":"))
    if 'topDir' in self.cmdCFG and self.cmdCFG["topDir"]:
      user_extra_dir.append(self.cmdCFG['topDir'])
    user_extra_dir.append(os.getenv('JUNOTESTROOT')+'/production')
    self.overallCFG.setAttr('DriverPath',user_extra_dir)
    logging.info("Driver Path: %s"% ':'.join(user_extra_dir))
    self.cwd = os.getcwd()
    if self.daemon:
      self._daemonize()

  def setTopDir(self, value):
    assert type(value) == str
    self.overallCFG.setAttr('topDir', value)

  def setBatchType(self, value):
    assert type(value) == str
    self.overallCFG.setAttr('batchType', value)

  def setIniFile(self, value):
    assert type(value) == str
    self.overallCFG.setAttr('initFile', value)

  def run(self):
    # Make sure workdir/subworkdir consistent
    this_workdir = None
    this_worksubdir = None

    if len(self.stepCFG):
        this_workdir = set(cfg[2]["workDir"] 
                            for cfg in self.stepCFG 
                            if cfg[2].get("workDir"))
        this_worksubdir = set(cfg[2]["workSubDir"] 
                            for cfg in self.stepCFG 
                            if cfg[2].get("workSubDir"))
    if this_workdir:
      this_workdir = list(this_workdir)[0]
    if this_worksubdir and len(this_worksubdir) > 1:
      logging.warning("Different workSubDir clash: %s"%this_worksubdir)

    ######################################
    # before running, parse ini File first
    ######################################
    if self.cmdCFG['iniFile'] and self.cmdCFG['name']:

      iniReader = configparser.RawConfigParser()
      iniReader.optionxform = str # make sure the option name is not changed
      iniReader.read(self.cmdCFG['iniFile']) 
      sec_name = self.cmdCFG['name']
      # try to check workflow:
      workflow_label = "workflow"
      if iniReader.has_option(sec_name, workflow_label):
          workflow_str = iniReader.get(sec_name, workflow_label)
          
          self.user_workflows = workflow_str.split()
      # try to check anaScrip
      anaWorkflow_label = "anaWorkflow"
      if iniReader.has_option(sec_name, anaWorkflow_label):
        workflow_str = iniReader.get(sec_name, anaWorkflow_label)
        logging.info("anaWorkflow: %s"%workflow_str)
        self.ana_workflow = workflow_str.split()
        # check ini tag
        for workstep in self.ana_workflow:
          if workstep not in ['detsim_ana','elecsim_ana','calib_ana','rec_ana', 'calib_woelec_ana', 'rec_woelec_ana']:
            print(('@Production: Can not recognize %s workstep, now we support[detsim_ana],[elecsim_ana],[calib_ana],[rec_ana], [calib_woelec_ana], [rec_woelec_ana]'%workstep))
          elif iniReader.has_option(sec_name,workstep):
            # workstep= script:subworkDir#
            anaScript = iniReader.get(sec_name,workstep).split()
            print(anaScript)
            for s in anaScript:
              subworkDir = None
              spath = None
              ana_cmd = None
              alltag = False
              if '#' in s:
                 alltag = True
                 s = s.replace('#','')
              if ':' in s:
                subworkDir = s.split(':')[1]
                spath = s.split(':')[0]
              else:
                spath = s
              if not subworkDir and this_worksubdir:
                subworkDir = list(this_worksubdir)[0]
              # make sure subworkDir in ana is same as others
              elif this_worksubdir and subworkDir and subworkDir not in this_worksubdir:
                logging.warning('The specified subworkDir:%s  is differnet from the prod script\'s subworkDir:%s, skip'%(subworkDir,this_worksubdir))
                continue

              # To reuse the same script, we put it into a separate section
              # The section name startswith '@' 
              if spath.startswith('@'):
                conf_file = None
                if iniReader.has_section(spath):
                  conf_file = self.cmdCFG['iniFile']
                elif 'extra_conf' in self.cmdCFG and self.cmdCFG['extra_conf']:
                  conf_file = self.cmdCFG['extra_conf']
                else:
                  for dd in self.overallCFG.getAttr('DriverPath'):
                    if dd and os.path.exists(dd+'/Extra_config.ini'):
                      conf_file = dd+'/Extra_config.ini'
                      break
                assert(os.path.exists(conf_file)),'config file doesn\'t exist!'
                print(('@production: get config file:%s'%conf_file))
                confReader = configparser.RawConfigParser()
                confReader.read(conf_file)
                ana_ini_sec = spath
                spath = confReader.get(ana_ini_sec, "script")
                # any cmd specified?
                # cmd could ref names under same section
                if confReader.has_option(ana_ini_sec, "cmd"):
                  ana_cmd = confReader.get(ana_ini_sec, "cmd")
                  print("ANA_CMD (raw): ", ana_cmd)
                  # We have predefined name:
                  # %(script)
                  ana_cmd = ana_cmd%dict(confReader.items(ana_ini_sec))
                  print("ANA_CMD : ", ana_cmd)
              
              if not spath and not ana_cmd:
                print("SKIP ", s)
                continue
              if spath:
                pattern = re.compile(r"[+('].*$")
                tmpath = os.path.expandvars(pattern.split(spath)[0])
                print(('DriverPath=%s'%self.overallCFG.getAttr('DriverPath')))
                if not os.path.exists(tmpath):
                  for dd in self.overallCFG.getAttr('DriverPath'):
                    if dd and os.path.exists(dd+'/'+tmpath):
                      spath = dd+'/'+spath
                      tmpath = dd+'/'+tmpath
                      break
                if not os.path.exists(tmpath):
                  logging.error("Can't find script '%s'."%tmpath)
                else:
                  logging.info('Find script %s'%tmpath)
              # For ROOT scripts, we need some after the file name.
              # Such as: 
              #   root -l -b -q myscript.C+
              #   root -l -b -q myscript.C'("arg1", "arg2")'
              
			  #if not os.path.exists(spath):
              #  print "%s no exist"%spath
              #  assert False
              print("@subworkDir = ",subworkDir)
              if ana_cmd:
                self.addAnaStep(ana_cmd, step=workstep, workDir=this_workdir, subworkDir=subworkDir, alltag = alltag)
              elif spath.endswith('.sh'):
                self.addAnaStep('bash %s'%spath,step=workstep, workDir=this_workdir, subworkDir=subworkDir, alltag = alltag)
              else:
                self.addAnaStep('root -b -q %s'%spath,step=workstep, workDir=this_workdir, subworkDir=subworkDir, alltag = alltag)
          else:
            logging.error("@Production: Can not find %s step script"%workstep)
    #import sys
    #sys.exit(0)
    need_external_dir = True

    ######################################
    # start running
    ######################################
    for sc in self.stepCFG:
      # when --no-prod and --no-ana skip
      if sc[0]=='batch' and not self.cmdCFG['prod']:
        continue
      elif sc[0] == 'analysis' and (not self.cmdCFG['ana'] and not self.cmdCFG['cmp']):
        continue
      # default CFG
      cfg = copy.deepcopy(self.overallCFG)
      # load from python script
      for k,v in list(sc[2].items()):
        if v:
          cfg.setAttr(k,v)
      # command line options
      for k,v in list(self.cmdCFG.items()):
        if v:
          cfg.setAttr(k,v)
      # load from ini file
      if self.cmdCFG['iniFile'] and self.cmdCFG['name']:

        iniReader = configparser.RawConfigParser()
        iniReader.optionxform = str # make sure the option name is not changed
        iniReader.read(self.cmdCFG['iniFile']) 

        # before loading the options in the section [sec_name],
        # load the options in the section [all] 
        for k, v in iniReader.items("all"):
          if v:
            cfg.setAttr(k, v)


        sec_name = self.cmdCFG['name']
        for k, v in iniReader.items(sec_name):
          if v:
            #print "LT", k,v
            cfg.setAttr(k, v)
      else:
        print("WARNING: ini: %s, section name: %s" %(self.cmdCFG['iniFile'], self.cmdCFG['name']))

      # before append it to self.steps, we need to check "extraArgs" is in self.user_workflows
      if self.user_workflows and self.cmdCFG['prod'] and sc[0]=='batch':
        #print('--user_workflow:%s'%self.user_workflows)
        extraArgs = cfg.getAttr("extraArgs")
        if extraArgs not in self.user_workflows:
          logging.warning("%s is not in %s, skip it"%(str(extraArgs), str(self.user_workflows)))
          continue
        # if now external-input-dir also exists, and it is in the first stage, set external-input-dir
        if need_external_dir: # only first time need it.
          need_external_dir = False
        else:
          cfg.setAttr("external-input-dir", None)

      # if it is default workflow (user_workflows = False),
      # and this is optional, skip it
      if not self.user_workflows and cfg.getAttr("optional"):
        continue
      if sc[0] == 'batch':
        if cfg.getAttr('batchType') == 'condor':
          self.steps.append(CondorJob(sc[1], cfg))
        elif cfg.getAttr('batchType') == 'lsf':
          self.steps.append(LSFJob(sc[1], cfg))
        elif cfg.getAttr('batchType') == 'pbs':
          self.steps.append(PBSJob(sc[1], cfg))
        else:
          print('unknown batch type')
          sys.exit(-1)
      else:
        self.steps.append(Analysis(sc[1], cfg))
    clearList = []
    failedList = []
    timeoutList = []

    logging.info("Workflows: %s"%self.steps)
    #import sys
    #sys.exit(0)

    
    #print('--script args cfg=%s'%cfg)
    for step in self.steps:
      ok = step.run()
      if not ok:
        print('Step failed')
        sys.exit(-1)
      if hasattr(step, 'getClearList'):
        clearList += step.getClearList()
      if hasattr(step, 'getFailedList'):
        failedList += step.getFailedList()
      if hasattr(step, 'getTimeoutList'):
        timeoutList += step.getTimeoutList()
      os.chdir(self.cwd)
    print('Production done')

    if timeoutList:
      print('List of timeout jobs:')
      for f in timeoutList: print(f)

    if self.ignore and failedList:
      print('List of failed jobs:')
      for f in failedList:
        print(f)
      sys.exit(0)

    if not self.keepTemp:
      print('Cleaning temporary files')
      for f in clearList:
        try:
          os.remove(f)
        except OSError:
          pass
      print('Done.')

  def addBatchStep(self, genScript, **kwa):
    # configure firstly
    self.stepCFG.append(['batch', genScript, kwa])

  def addAnaStep(self, cmd, **kwa):
    logging.debug("@Production: add new anaStep: %s"%cmd)
    self.stepCFG.append(['analysis', cmd, kwa])

  def _daemonize(self):
    print('Monitoring process is daemonized, log info can be find in: %s' % self.logFile)
    try:
      pid = os.fork()
      if pid > 0:
        sys.exit(0)
    except OSError as e:
      sys.stderr.write("fork #1 failed: (%d) %s\n" % (e.errno, e.strerror))
      sys.exit(1)

    #os.umask(0)
    os.setsid()

    try:
      pid = os.fork()
      if pid > 0:
        sys.exit(0)
    except OSError as e:
      sys.stderr.write("fork #2 failed: (%d) %s\n" % (e.errno, e.strerror))
      sys.exit(1)

    for f in sys.stdout, sys.stderr: f.flush()
    so = open(self.logFile, 'a+')
    se = open(self.logFile, 'a+', 0)
    os.dup2(so.fileno(), sys.stdout.fileno())
    os.dup2(se.fileno(), sys.stderr.fileno())

  def _configureArg(self):
    argParser = argparse.ArgumentParser()
    argParser.add_argument("--batch", dest="batch", choices = ["condor", "lsf", "pbs"], default = "condor", help="Batch job backend")
    argParser.add_argument("--topDir", dest="topDir", help="Top directory to do the production")
    argParser.add_argument("--log", dest="log", help="Log file for the production")
    argParser.add_argument("--loglevel", help="Log Level", default="info", choices=["debug", "info"])
    argParser.add_argument("--ini", dest="ini", help="ini file for the production")
    argParser.add_argument("--name", dest="name", help="name of the production")
    argParser.add_argument("--test", dest="test", action="store_true", help="For testing, jobs will not be submitted")
    argParser.add_argument("--daemon", dest="daemon", action="store_true", help="run as a daemon.")
    argParser.add_argument("--no-daemon", dest="daemon", action="store_false", help="run as a non daemon.")
    argParser.add_argument("--temp", dest="temp", action="store_true", help="Keep the temporary files")
    argParser.add_argument("--no-temp", dest="temp", action="store_false", help="Do not keep the temporary files")
    argParser.add_argument("--ana", dest="ana", action="store_true", help="Run the analysis step")
    argParser.add_argument("--no-ana", dest="ana", action="store_false", help="Do not run the analysis step")
    argParser.add_argument("--ignore-failure", dest="ignore", action="store_true", help="Ignore failed jobs and continue production")
    argParser.add_argument("--no-ignore-failure", dest="ignore", action="store_false", help="Do not ignore failed jobs")
    argParser.add_argument("--benchmark", action="store_true", help="running benchmark on specific machines")
    
    argParser.add_argument("--prod", dest="pord", action="store_true", default=True, help="Run the production step")
    argParser.add_argument("--no-prod", dest="prod", action="store_false", help="Do not run the production step")
    argParser.add_argument("--cmp", dest="cmp", action="store_true", default=True, help="Run the comparison step")
    argParser.add_argument("--no-cmp", dest="cmp", action="store_false", help="Do not run the comparison step")
    argParser.add_argument("--econf", dest="extra_conf", help = "extra configuration for ini file")
    argParser.add_argument("--maxTime", dest="maxTime", default="-1", help="Time limitation (s) for batch jobs. -1 means no limitation.")
    argParser.add_argument("--walltime=", dest="walltime", default="72:00:00", help="Time limitation (hh:mm:ss) for PBS batch jobs.")
    args = argParser.parse_args()

    numeric_level = getattr(logging, args.loglevel.upper(), None)
    logging.basicConfig(level=numeric_level)

    self.cmdCFG['batchType'] = args.batch
    self.cmdCFG['topDir'] = args.topDir 
    self.cmdCFG['test'] = args.test
    self.cmdCFG['iniFile'] = args.ini
    self.cmdCFG['name'] = args.name
    self.cmdCFG['ana'] = args.ana
    self.cmdCFG['prod'] = args.prod
    self.cmdCFG['cmp'] = args.cmp
    self.cmdCFG['ignoreFailure'] = args.ignore
    self.cmdCFG['benchmark'] = args.benchmark
    self.cmdCFG['extra_conf'] = args.extra_conf
    self.cmdCFG['maxTime'] = args.maxTime
    self.cmdCFG['walltime'] = args.walltime
    if args.log:
      self.logFile = args.log
    self.daemon = args.daemon
    self.keepTemp = args.temp
    self.ignore = args.ignore
