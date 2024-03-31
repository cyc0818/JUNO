#!/usr/bin/python

from JunoTest import *


if __name__ == "__main__":

  Co60 = Production()

  scriptDir = '$JUNOTESTROOT/production/Co60'

  Co60.addBatchStep('source %s/gen-Co60.sh' % scriptDir, workDir = 'Co60')

  Co60.run()
