#!/usr/bin/env python

from JunoTest import *


if __name__ == "__main__":

  Alpha_center = Production()
  import os.path
  scriptDir = os.path.dirname(os.path.realpath(__file__))
  print(scriptDir)

  Alpha_center.addBatchStep('bash %s/gen-alpha-uniform.sh' % scriptDir, extraArgs="detsim",  workDir = 'Alpha', workSubDir='uniform')
  Alpha_center.addBatchStep('bash %s/gen-alpha-uniform.sh' % scriptDir, extraArgs="elecsim",  workDir = 'Alpha', workSubDir='uniform')
  Alpha_center.addBatchStep('bash %s/gen-alpha-uniform.sh' % scriptDir, extraArgs="calib",  workDir = 'Alpha', workSubDir='uniform')
  Alpha_center.addBatchStep('bash %s/gen-alpha-uniform.sh' % scriptDir, extraArgs="rec",  workDir = 'Alpha', workSubDir='uniform')

  Alpha_center.run()
