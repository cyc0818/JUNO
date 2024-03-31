#!/usr/bin/python

from JunoTest import *


if __name__ == "__main__":
  
  IBD_Pro = Production()

  scriptDir = '$JUNOTESTROOT/production/IBD-BasicDistribution'

  IBD_Pro.addBatchStep('source %s/gen-eplus.sh' % scriptDir, workDir = 'IBD/IBD-eplus')
  IBD_Pro.addBatchStep('source %s/gen-neutron.sh' % scriptDir, workDir = 'IBD/IBD-neutron')
  IBD_Pro.addAnaStep('BasicDistribution.exe -eplus -listname ../IBD-eplus/sample-eplus.txt', workDir = 'IBD/BasicDist', plotFile='eplus.root', cmpOutput='cmp_eplus.root')
  IBD_Pro.addAnaStep('BasicDistribution.exe -neutron -listname ../IBD-neutron/sample-neutron.txt', workDir = 'IBD/BasicDist', plotFile='neutron.root', cmpOutput='cmp_neutron.root')

  IBD_Pro.run()
