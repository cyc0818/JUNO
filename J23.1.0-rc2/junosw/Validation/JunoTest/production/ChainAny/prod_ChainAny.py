#!/usr/bin/env python

from JunoTest import *


if __name__ == "__main__":

  Positron_center = Production()
  import os.path
  scriptDir = os.path.dirname(os.path.realpath(__file__))
  print(scriptDir)

  cmd = 'bash %s/gen-position.sh' % scriptDir

  Positron_center.addBatchStep(cmd, extraArgs="detsim",  workDir = 'Positron', workSubDir='center')
  Positron_center.addBatchStep(cmd, extraArgs="elecsim", workDir = 'Positron', workSubDir='center')
  Positron_center.addBatchStep(cmd, extraArgs="calib",   workDir = 'Positron', workSubDir='center')
  Positron_center.addBatchStep(cmd, extraArgs="rec",     workDir = 'Positron', workSubDir='center')
  Positron_center.addBatchStep(cmd, optional=True, extraArgs="calib-woelec", workDir = 'Positron', workSubDir='center')
  Positron_center.addBatchStep(cmd, optional=True, extraArgs="rec-woelec",   workDir = 'Positron', workSubDir='center')

  Positron_center.run()
