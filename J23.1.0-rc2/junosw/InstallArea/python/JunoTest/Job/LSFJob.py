import sys
from .BatchJob import *
from JunoTest.Utils.shellUtil import *

class LSFJob(BatchJob):

  def __init__(self, genCMD, cfg):
    BatchJob.__init__(self, genCMD, cfg)
    self.queue = cfg.getAttr('LSFQueue')
    self.memoryLimit = cfg.getAttr('LSFMemoryLimit')
    self.subPrefix = "run-"
    self.subSuffix = ".sh"

  def _checkStatus(self):
    result = {}
    allstat = GetLSFJobStat()
    if not allstat: return result
    allstat = allstat.split('\n')[1:-1]
    for s in allstat:
      if not s: continue
      sl = s.split()
      id = sl[0]
      status = sl[2]
      if status == 'DONE':
        # success
        result[id] = JobStatus.DONE
      elif status == 'EXIT':
        # fail
        result[id] = JobStatus.FAILED
      else:
        # other
        result[id] = JobStatus.UNDONE
    return result

  def _submit(self, script):
    log = os.path.join(os.path.dirname(script), self.logPrefix +os.path.basename(script)[len(self.subPrefix):-len(self.subSuffix)] + self.logSuffix)
    id = SubmitLSFJob(script, log, self.queue, self.memoryLimit)
    if not id:
      print("ERROR: Failed to submit job %s" % script)
      sys.exit(-1)
    return id
