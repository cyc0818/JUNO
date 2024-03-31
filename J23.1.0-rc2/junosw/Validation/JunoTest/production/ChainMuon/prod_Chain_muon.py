#!/usr/bin/env python

from JunoTest import *


if __name__ == "__main__":

  Muon = Production()
  import os.path
  scriptDir = os.path.dirname(os.path.realpath(__file__))
  print(scriptDir)

  Muon.addBatchStep('bash %s/gen-muon.sh' % scriptDir, extraArgs="detsim",  workDir = 'Muon')
  Muon.addBatchStep('bash %s/gen-muon.sh' % scriptDir, extraArgs="elecsim", workDir = 'Muon')
  Muon.addBatchStep('bash %s/gen-muon.sh' % scriptDir, extraArgs="calib",   workDir = 'Muon')
  Muon.addBatchStep('bash %s/gen-muon.sh' % scriptDir, extraArgs="rec",     workDir = 'Muon')

  Muon.run()
