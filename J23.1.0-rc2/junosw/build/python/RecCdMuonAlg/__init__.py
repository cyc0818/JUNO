import Sniper as sn
import types
sn.loadDll("libCLHEPDict.so")
sn.loadDll("libRecCdMuonAlg.so")

def useRecTool(self, name):
    rectool = self.createTool(name)
    self.property("RecTool").set(name)
    self.rectool=rectool


def createAlg(task, name="RecCdMuonAlg"):
    alg = task.createAlg(name)
    alg.useRecTool=types.MethodType(useRecTool, alg)
    return  alg
    
