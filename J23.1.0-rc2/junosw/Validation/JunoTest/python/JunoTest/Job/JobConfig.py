import types

class ProductionConfig:

  defaults = {
               'verbose' : False,
               'workDir' : None,    # Notes: the final path is:
               'workSubDir' : None, #   topDir / workDir / workSubDir / tag
               'topDir' : None,     #
               'external-input-dir': None, # allow user specify input from other directories
                                           # guess input file in this order:
                                           #  * EID / workDir / workSubDir / tag / detsim / detsim-xxx.root
                                           #  * EID           / workSubDir / tag / detsim / detsim-xxx.root
                                           #  * EID                        / tag / detsim / detsim-xxx.root
                                           #  * EID                              / detsim / detsim-xxx.root
                                           #  *                                    detsim / detsim-xxx.root
                                           #                                      |  this part is default  |
               'maxRetry' : 2,
               'optional' : False, # if optional is True and user don't use it in workflow, this step will be disabled.
               'batchType' : 'condor',
               'condorSched' : 'scheduler@schedd06.ihep.ac.cn',
               'condorPool' : 'condor04.ihep.ac.cn',
               'condor-template': None, # the condor extra template file name
               'LSFQueue' : 'juno',
               'LSFMemoryLimit': 4000000,
               'PBSWalltime': '72:00:00',
               'benchmark': False,
               'target-os' : None, # SL6 or SL7
               'ana' : True,
               'refDir' : None,
               'plotFile' : None,
               'cmpOutput' : None,
               'histTestMeth' : 'Kolmogorov',
               'histTestCut' : 0.9,
               'interval' : 10,
               'fatalPattern' : None,
               'log' : 'log.txt',
               'test' : False, 
               'initFile' : None,
               'temp' : True,
               'ignoreFailure': True,
               'maxTime': -1, # Time limit (s) of batch jobs. Job will be dropped once its running time exceeded. -1 means no limitation.
             }

  def __init__(self):
    self.config = dict([(k,v) for (k,v) in list(ProductionConfig.defaults.items())])

  def update(self, **kwa):
    self.config.update(kwa)

  def setAttr(self, name, value):
    assert type(name) == str, "ERROR: attribute must be of String type!"
    self.config[name] = value

  def getAttr(self, name):
    if name in self.config:
      return self.config[name]
    return None

