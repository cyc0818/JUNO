import time
import os,sys
from JunoTest.Utils.shellUtil import *
from JunoTest.Process.Parser import Parser

# compatible between 2 and 3
try:
  import configparser
except ImportError:
  import ConfigParser as configparser

class JobInfo:
  def __init__(self, id, sub, log, start):
    self.nRetry = 0
    self.startTime = start
    self.id = id
    self.subScript = sub
    self.logFile = log

class JobStatus:
  (DONE, UNDONE, FAILED) = list(range(0,3))

class BatchJob:
  def __init__(self, genCMD, cfg):
    self.topDir = cfg.getAttr('topDir')
    self.workDir = cfg.getAttr('workDir')
    self.workSubDir = cfg.getAttr('workSubDir')
    self.maxRetry = cfg.getAttr('maxRetry')
    self.monitorInterval =  cfg.getAttr('interval')
    self.testFlag = cfg.getAttr('test')
    self.name = cfg.getAttr('name')
    self.iniFile = cfg.getAttr('iniFile')
    self.ignore = cfg.getAttr('ignoreFailure')
    self.maxTime = float(cfg.getAttr('maxTime'))
    self.walltime = (cfg.getAttr('walltime'))
    self.logPrefix = 'log-'
    self.logSuffix = '.txt'
    self.submittedFlag = '.submitted'
    self.parser = Parser(cfg)
    self.notYetFinished = []
    self.allSubScript = []
    self.clearList = []
    self.failedList = []
    self.timeoutList = []
    self.args = ' '
    if self.iniFile:
      self._parseIni()
    self.genCMD = genCMD + self.args + ' --' + cfg.getAttr('batchType')
    # benchmark?
    if cfg.getAttr("benchmark"):
      self.genCMD += ' --benchmark '
    if cfg.getAttr("target-os"):
      self.genCMD += ' --target-os "%s"'%cfg.getAttr("target-os")
    if cfg.getAttr("condor-template"):
      self.genCMD += ' --condor-template "%s" '%cfg.getAttr("condor-template")
    # external input dir
    if cfg.getAttr("external-input-dir"):
      self.genCMD += ' --external-input-dir "%s" '%cfg.getAttr("external-input-dir")
    # workdir and worksubdir
    if self.workDir:
      self.genCMD += ' --workdir "%s" '%self.workDir
    if self.workSubDir:
      self.genCMD += ' --worksubdir "%s" '%self.workSubDir
    # extra sub command
    self.extraArgs = "" # it is our staging alias
    if cfg.getAttr("extraArgs"):
        self.extraArgs = cfg.getAttr("extraArgs")
        self.genCMD += " " + cfg.getAttr("extraArgs")

  def run(self):
    print("Start to run CondorJob step: %s" % self.genCMD)
    print("Working directory is: " + (self.topDir or os.getcwd()) + '/' + (self.workDir or ''))
    self._gotoWorkDir()
    self._genSubScript()
    self._submitJobs()
    # monitor && verify && resubmit
    while True:
      if not self.testFlag:
        time.sleep(self.monitorInterval)
      jobStatus = self._checkStatus()
      now = time.time()
      for job in self.notYetFinished:
        # Check job status
        if job.id not in jobStatus:
          #Job done
          status = JobStatus.DONE
        else:
          status = jobStatus[job.id]

        # Deal with the job
        success = True
        if status == JobStatus.DONE:
          # Job done
          okay, what = self._parseLog(job.logFile)
          if okay:
            print('Job: %s ends successfully' % job.id)
            self.notYetFinished.remove(job)
          else:
            print(what)
            success = False
        if status == JobStatus.FAILED or not success:
          # Job failed
          print('Job: %s (%s) failed' % (job.id, job.logFile))
          if job.nRetry >= self.maxRetry:
            print('ERROR: Max retry times reached, job failed!!')
            if self.ignore:
              self.failedList.append(job.subScript)
              self.notYetFinished.remove(job)
            else:
              return False
          else:
            # re-submit
            print('Resubmitting...')
            os.chdir(os.path.dirname(job.subScript))
            job.id = self._submit(job.subScript)
            job.startTime = time.time()
            if os.path.exists(job.logFile):
              os.rename(job.logFile, job.logFile + '_' + str(job.nRetry))
            job.nRetry = job.nRetry + 1
        # Check time
        if self.maxTime != -1 and (time.time() - job.startTime) > self.maxTime:
          self.notYetFinished.remove(job)
          self.timeoutList.append(job.subScript)
            
      if not len(self.notYetFinished):
        print('All jobs end successfully!')
        return True

  def _gotoWorkDir(self):
    try:
      if self.topDir:
        MakeAndCD(self.topDir)
      if self.workDir:
        MakeAndCD(self.workDir)
      if self.workSubDir:
        MakeAndCD(self.workSubDir)
    except OSError:
      print('ERROR: Failed to make top directory')
      sys.exit(-1)

  def _genSubScript(self):
    print('Generating submitting script...')
    if 0 != os.system(self.genCMD):
      print('ERROR: failed to generate submitting script!!')
      sys.exit(-1)
    print('Done.')

  def _submitJobs(self):
    print('Start to submit jobs...')
    cwd = os.getcwd()
    self._recursiveGetSubScripts(cwd)
    if self.testFlag:
      print("Testing flag detected, jobs will not be submitted")
    print("Current workding directory: %s" %cwd)
    for filename in self.allSubScript:
      dir = os.path.dirname(filename)
      b = os.path.basename(filename)
      # pushd
      _cwd = os.getcwd()
      os.chdir(dir)
      print("submit job %s@%s"%(filename, dir))
      bn = os.path.basename(filename)[len(self.subPrefix):-len(self.subSuffix)]
      logname = os.path.join(dir, self.logPrefix + bn + self.logSuffix)
      if self.testFlag:
        print("[testing] submit job %s -> %s"%(filename, logname))
        continue
      jobID = self._submit(filename)
      self.notYetFinished.append(JobInfo(jobID, filename, logname, time.time()))

      # mark the submitted job
      temp = filename + self.submittedFlag
      tempFile = open(temp, 'w')
      tempFile.write(str(jobID))
      tempFile.close()
      self.clearList.append(temp)
      os.chdir(_cwd)
      # popd

      if 0 == len(self.notYetFinished):
        print('ERROR: no job submitted!')
        sys.exit(-1)
    #os.chdir(cwd)
    print('%d job(s) submitted in total' % len(self.notYetFinished))

  def _recursiveGetSubScripts(self, dir):
    all_files_dirs = os.listdir(dir)
    for f in all_files_dirs:
      p = os.path.join(dir, f)
      if f.startswith(self.subPrefix) and f.endswith(self.subSuffix) and not os.path.exists(p + self.submittedFlag):
        self.allSubScript.append(p)
      elif os.path.isdir(p):
        self._recursiveGetSubScripts(p)

  def _parseLog(self, logFile):
    return self.parser.parseFile(logFile)

  def _parseIni(self):
    assert os.path.exists(self.iniFile), 'ERROR: Cannot find production intialization file: %s' % self.iniFile
    self.iniReader = configparser.RawConfigParser()
    self.iniReader.read(self.iniFile)
    self._parseIniSection('all')
    if self.name:
      self._parseIniSection(self.name)

  def _parseIniSection(self, name):
    deal = { 'seed': ' --seed ',
             'evtmax': ' --evtmax ',
             'njobs': ' --njobs ',
             'setup': ' --setup ',
             'tags': ' --tags ',
           }
    # skip some options, which is used by high level command
    extra_user_options_list = ["extra-mixing-tags"]
    extra_user_options_list += ["detsim-mode"]
    extra_user_options_list += ["detsim-submode"]
    extra_user_options_list += ["elecsim-mode"]
    extra_user_options_list += ["calib-mode"]
    extra_user_options_list += ["rec-mode"]
    extra_user_options_list += ["rates"]

    try:
      result = self.iniReader.items(name)
      for item in result:
        # FIXME: it is a pain to handle command line options when using os.system. 
        #        consider using subprocess instead.
        if deal.get(item[0]): self.args = self.args + deal.get(item[0]) + '"%s"'%item[1]
        elif item[0] in extra_user_options_list:
            self.args += ' --%s "%s" '%(item[0], item[1])
        else: # if can't find in deal, try to guess
            continue
    except configparser.NoSectionError:
      # No section, do nothing
      pass

  def getClearList(self):
    return self.clearList

  def getFailedList(self):
    return self.failedList

  def getTimeoutList(self):
    return self.timeoutList

  def __repr__(self):
    return "Job@%s"%self.extraArgs
