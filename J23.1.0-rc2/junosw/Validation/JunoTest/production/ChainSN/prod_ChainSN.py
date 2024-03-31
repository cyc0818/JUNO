#!/usr/bin/env python

from JunoTest import *


if __name__ == "__main__":

  Supernova_center = Production()
  import os.path
  scriptDir = os.path.dirname(os.path.realpath(__file__))
  print(scriptDir)

  cmd = 'bash %s/gen-position.sh' % scriptDir

  Supernova_center.addBatchStep(cmd, extraArgs="detsim",  workDir = 'Supernova', workSubDir='center')
  Supernova_center.addBatchStep(cmd, extraArgs="elecsim", workDir = 'Supernova', workSubDir='center')
  Supernova_center.addBatchStep(cmd, extraArgs="calib",   workDir = 'Supernova', workSubDir='center')
  Supernova_center.addBatchStep(cmd, extraArgs="rec",     workDir = 'Supernova', workSubDir='center')
  Supernova_center.addBatchStep(cmd, optional=True, extraArgs="calib-woelec", workDir = 'Supernova', workSubDir='center')
  Supernova_center.addBatchStep(cmd, optional=True, extraArgs="rec-woelec",   workDir = 'Supernova', workSubDir='center')

  Supernova_center.run()
