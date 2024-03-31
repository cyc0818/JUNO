import types
import subprocess
from JunoTest.Utils.TestConfig import TestConfig
from JunoTest.Utils.shellUtil import *
from JunoTest.Process.Process import Process
from JunoTest.PlotTester.PlotTester import PlotTester

class Analysis:

  def __init__(self, cmd, cfg):
    self.cmd = cmd
    self.cfg = cfg
    self.clearList = []
    self.anacfg = TestConfig()

  def run(self):
    if not self.cfg.getAttr('ana') and not self.cfg.getAttr('cmp'):
      print("Skip Analysis step: %s" % self.cmd)
      return True
    print("Start to run Analysis step: %s" % self.cmd)
    self.anacfg.setAttr('histTestMeth', self.cfg.getAttr('histTestMeth'))
    self.anacfg.setAttr('histTestCut', self.cfg.getAttr('histTestCut'))
    self.anacfg.setAttr('cmpOutput', self.cfg.getAttr('cmpOutput'))
    self.anacfg.setAttr('step',self.cfg.getAttr('step'))
    # force genLog
    self.anacfg.setAttr('genLog', True)
    refList = []
    refDir = self.cfg.getAttr('refDir')
    plotFile = self.cfg.getAttr('plotFile')
    if refDir and plotFile:
      if type(plotFile) == str:
        plotFile = [plotFile]
      for f in plotFile:
        refList.append(refDir + '/' + f)
    print(("@Analysis plotRef=%s"%refList))
    self.anacfg.setAttr('plotRef', refList)

    topDir = self.cfg.getAttr('topDir')
    workDir = self.cfg.getAttr('workDir')
    
    # expand tags
    tags_raw = self.cfg.getAttr('tags').replace('\n',' ')
    tags_after = subprocess.check_output(["bash", "-c", 'eval echo %s'%tags_raw]).decode().strip()

    taglist = tags_after.split()
    print("taglist: ", taglist)
    #print "Working directory is: " + (topDir or os.getcwd()) + '/' + (workDir or '')
    if topDir:
      MakeAndCD(topDir)
    if workDir:
      MakeAndCD(workDir)
    subworkDirlist = os.listdir(".")
    swDir = self.cfg.getAttr('subworkDir')
    key = 'JUNOTEST_TOPDIR_INPUT'
    val = os.path.abspath(os.curdir)
    os.putenv(key,val)
    curdir = os.getcwd()
    if not swDir:
      # if there is no subworkDir in prod script or .ini file,
      #        we just skip this directory, and run job under workDir 
      self.execuate_ana(taglist, refList, self.cfg.getAttr('alltag'))
      os.chdir(curdir)
    elif swDir in subworkDirlist:
      if os.path.isdir(swDir):
        os.chdir(swDir)
        self.execuate_ana(taglist,refList, self.cfg.getAttr('alltag'))
        os.chdir(curdir)
    else:
      print(('@Analysis: Can not find subworkDir %s'%self.cfg.getAttr('subworkDir')))
      return False

    print('Analysis step done')
    return True

  def getClearList(self):
    return self.clearList

  def setEnvPath(self, prepath,workstep):
    tagsublist = os.listdir(prepath)
    if workstep[:-4] in tagsublist:
      key = "JUNOTEST_%s_INPUT"%workstep.upper()
      val = "%s/%s"%(prepath, workstep[:-4])
      #os.putenv(key,val)
      os.environ[key] = val;
#      try:
#        print("@Analysis: add new input env path %s=%s"%(key,os.getenv[key]))
#      except KeyError:
#        print('@Analysis error: put env key=%s failed'%key)
      key = "JUNOTEST_%s_OUTPUT"%workstep.upper()
      val = "%s/%s"%(prepath, workstep)
      #os.putenv(key,val)
      os.environ[key] = val;
#      try:
#        print("@Analysis: add new output env path %s=%s"%(key,os.getenv[key]))
#      except KeyError:
#        print('@Analysis error: put env key=%s failed'%key)
    else:
      print(('@setEnvPath: Can not find workstep [%s] input dir %s'%(workstep,workstep[:-4])))

  def execuate_ana(self, taglist, refList, alltag):
    refDir = self.cfg.getAttr('refDir')

    # Sometimes tag is not just a single directory.
    tagDir = []
    for tag in taglist:
      if os.path.exists(tag):
        tagDir.append(tag)
    key = "JUNOTEST_TAG_LIST"
    val = ''
    for item in tagDir:
      val+=item+':'
    os.environ[key] = val
    if alltag:
      if self.cfg.getAttr('step') not in os.listdir('.'):
        os.mkdir(self.cfg.getAttr('step'))
      if "JUNOTEST_%s_INPUT"%self.cfg.getAttr('step').upper() in os.environ:
        del os.environ["JUNOTEST_%s_INPUT"%self.cfg.getAttr('step').upper()]
      key = "JUNOTEST_%s_OUTPUT"%self.cfg.getAttr('step').upper()
      val = "%s/%s"%(os.getcwd(),self.cfg.getAttr('step'))
      os.environ[key] = val
      #print('set %s=%s'%(key,val))
      print(('@Analysis: execute current dir=%s'%os.getcwd()))
      if not self.cfg.getAttr('ana'):
        print("Analysis step skip")
      else :
        #self.anacfg.setAttr('logName', self.cfg.getAttr('step'))
        process = Process('Analysis_%s'%self.cfg.getAttr('step'), self.cmd, self.anacfg)
        process.run()
        ok, what = process.outcome()
        if not ok:
          print('Analysis process failed: %s; plot comparison skip' % (what))
          #print what
          return
      # use plotTester
      if refDir and self.cfg.getAttr('cmp'):
        print('Start to run PlotTester')
        self.plotTester = PlotTester(self.anacfg, refList)
        self.plotTester.run()
      else:
        print('Skip Plot Comparison step')
      return

    curdir = os.path.abspath(os.curdir)
    for tD in tagDir:
      os.chdir(curdir)
      MakeAndCD(tD)  # cd to tags dir
      if self.cfg.getAttr('step') not in os.listdir('.'):
        os.mkdir(self.cfg.getAttr('step'))
      #MakeAndCD(self.cfg.getAttr('step'))
      print(('@Analysis: execute current dir=%s'%os.getcwd()))
      #self.setEnvPath(os.path.abspath('..'), self.cfg.getAttr('step'))
      self.setEnvPath(os.getcwd(), self.cfg.getAttr('step'))
      if not self.cfg.getAttr('ana'):
        print("Analysis step skip")
      else :
        #self.anacfg.setAttr('logName', self.cfg.getAttr('step'))
        process = Process('Analysis_%s'%self.cfg.getAttr('step'), self.cmd, self.anacfg)
        process.run()
        ok, what = process.outcome()
        if not ok:
          print('Analysis tag %s process failed: %s; plot comparison skip' % (tD,what))
          #print what
          continue
      # use plotTester
      if refDir and self.cfg.getAttr('cmp'):
        print('Start to run PlotTester')
        self.plotTester = PlotTester(self.anacfg, refList)
        self.plotTester.run()
      else:
        print('Skip Plot Comparison step')
