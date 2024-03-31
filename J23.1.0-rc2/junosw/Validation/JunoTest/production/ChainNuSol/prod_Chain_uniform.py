#!/usr/bin/env python

from JunoTest import *


if __name__ == "__main__":

  Positron_center = Production()
  import os.path
  scriptDir = os.path.dirname(os.path.realpath(__file__))
  print(scriptDir)

  #Positron_center.addBatchStep('bash %s/gen-gdml.sh' % scriptDir, workDir = 'Positron/center')
  Positron_center.addBatchStep('bash %s/gen-uniform.sh' % scriptDir, extraArgs="detsim",  workDir = 'NuSol', workSubDir='uniform')
  Positron_center.addBatchStep('bash %s/gen-uniform.sh' % scriptDir, extraArgs="elecsim", workDir = 'NuSol', workSubDir='uniform')
  Positron_center.addBatchStep('bash %s/gen-uniform.sh' % scriptDir, extraArgs="calib",   workDir = 'NuSol', workSubDir='uniform')
  Positron_center.addBatchStep('bash %s/gen-uniform.sh' % scriptDir, extraArgs="rec",     workDir = 'NuSol', workSubDir='uniform')
  Positron_center.addBatchStep('bash %s/gen-uniform.sh' % scriptDir, optional=True, extraArgs="calib-woelec", workDir = 'NuSol', workSubDir='uniform')
  Positron_center.addBatchStep('bash %s/gen-uniform.sh' % scriptDir, optional=True, extraArgs="rec-woelec",   workDir = 'NuSol', workSubDir='uniform')

  #Positron_center.addAnaStep('root -b -q %s/RecAnalysis-center.c' % scriptDir, workDir = 'Positron/center')
  #Positron_center.addAnaStep('root -b -q %s/resolution-center.c' % scriptDir, workDir = 'Positron/center')

  Positron_center.run()
