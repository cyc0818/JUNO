import sys
from .BatchJob import *
from JunoTest.Utils.shellUtil import *

class PBSJob(BatchJob):

  def __init__(self, genCMD, cfg):
    BatchJob.__init__(self, genCMD, cfg)
    self.queue = cfg.getAttr('PBSQueue')
    self.walltime = cfg.getAttr('PBSWalltime')
    self.subPrefix = "run-"
    self.subSuffix = ".sh"

  def _checkStatus(self):
    result = {}
    allstat = GetPBSJobStat()
    if not allstat: return result
    #allstat = allstat.split('\n')[1:-1]
    allstat = allstat.split('\n')[4:-1]
    for s in allstat:
      if not s: continue
      sl = s.split()
      idtemp = sl[0].split(".")
      id = idtemp[0]
      status = sl[-2]
      if status == 'C':
        # success
        result[id] = JobStatus.DONE
      elif status == 'E':
        # fail
        result[id] = JobStatus.FAILED
      else:
        # other
        result[id] = JobStatus.UNDONE
    return result

  def _submit(self, script):
    log = os.path.join(os.path.dirname(script), self.logPrefix +os.path.basename(script)[len(self.subPrefix):-len(self.subSuffix)] + self.logSuffix)
    id = SubmitPBSJob(script, log, self.walltime)
    if not id:
      print("ERROR: Failed to submit job %s" % script)
      sys.exit(-1)
    return id
