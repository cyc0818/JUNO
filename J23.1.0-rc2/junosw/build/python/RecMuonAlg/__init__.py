import Sniper as sn
import types
sn.loadDll("libCLHEPDict.so")
sn.loadDll("libRecMuonAlg.so")

def useRecTool(self, name):
    rectool = self.createTool(name)
    self.property("RecTool").set(name)
    self.rectool=rectool


def createAlg(task, name="RecMuonAlg"):
    alg = task.createAlg(name)
    alg.useRecTool=types.MethodType(useRecTool, alg)
    return  alg
    
