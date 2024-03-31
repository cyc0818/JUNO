import sys
from .BatchJob import BatchJob, JobStatus
from JunoTest.Utils.shellUtil import *

class CondorJob(BatchJob):

  def __init__(self, genCMD, cfg):
    BatchJob.__init__(self, genCMD, cfg)
    self.sched = cfg.getAttr('condorSched')
    self.pool  = cfg.getAttr('condorPool')
    self.subPrefix = "sub-"
    self.subSuffix = ".condor"

  def _checkStatus(self):
    stat = GetCondorJobStat(self.sched, self.pool)
    result = {}
    sl = stat.split('\n')
    for r in sl:
      rl = r.split()
      if len(rl):
        if rl[1] == 3:
          #Failed
          result[rl[0]] = JobStatus.FAILED
        elif rl[1] == 4:
          #Done
          result[rl[0]] = JobStatus.DONE
        else:
          #Other
          result[rl[0]] = JobStatus.UNDONE
    return result

  def _submit(self, script):
    id = SubmitCondorJob(script)
    if not id:
      print('ERROR: Failed to submit job: %s' % script)
      sys.exit(-1)
    # For condor job, remove the submition script
    self.clearList.append(script)
    return id
