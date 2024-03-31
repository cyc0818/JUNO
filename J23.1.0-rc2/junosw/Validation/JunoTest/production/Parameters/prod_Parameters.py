#!/usr/bin/python

from JunoTest import *


if __name__ == "__main__":

  parameter = Production()

  scriptDir = '$JUNOTESTROOT/production/Parameters'

  parameter.addBatchStep('source %s/gensim.sh' % scriptDir, workDir = 'Parameters')
  parameter.addAnaStep('root -b -q %s/draw_from_user.C' % scriptDir, workDir = 'Parameters', plotFile='Parameters.root', cmpOutput='cmp_parameters.root')
  parameter.run()
