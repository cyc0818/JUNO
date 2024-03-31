#!/usr/bin/env python

from JunoTest import *


if __name__ == "__main__":

  TT_production = Production()
  import os.path
  scriptDir = os.path.dirname(os.path.realpath(__file__))
  print(scriptDir)

  cmd = 'bash %s/gen-muon.sh' % scriptDir

  TT_production.addBatchStep(cmd, extraArgs="detsim",       workDir = 'TT', workSubDir='Muon')
  TT_production.addBatchStep(cmd, extraArgs="calib-woelec", workDir = 'TT', workSubDir='Muon')
  TT_production.addBatchStep(cmd, extraArgs="rec-woelec",   workDir = 'TT', workSubDir='Muon')

  TT_production.run()
