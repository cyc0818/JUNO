#!/bin/env python

import os, sys, time

def MakeDir(dir):
  if not os.path.isdir(dir):
    try :
      os.makedirs(dir)
    except OSError:
      print ('Failed to make dir :' + dir)
      sys.exit(-1)

def gen_XXX_sh(seqno, energy):
  seed = int(seqno)
  seed2 = int(energy)
  seqno = str(seqno)
  energy = str(energy)



  infile = '/dybfs2/users/yuchincheng/workdir/juno/data/ForMLTraining/KE_' + energy + '.0MeV/Truth/Truth_user_' + seqno + '.root'
  jobname = jobdir + '/run_' + seqno + '.sh'

  f = open(jobname,'w')
  f.write('#!/bin/bash\n')
  f.write('source /junofs/users/yuchincheng/J23.1.0-rc2/bashrc\n')
  f.write('/junofs/users/yuchincheng/NTU/ML/PixelConversion/PixelConversion %s %s\n' %(infile, workdir))

  f.close()


workdir = ''
jobdir = ''

for i in range(0,10):
    workdir = '/dybfs2/users/yuchincheng/workdir/juno/data/ML_csv/KE_' + str(i) + '.0MeV'
    jobdir = workdir + '/job'
    MakeDir(jobdir)

    os.chdir(jobdir)

    subname = jobdir + '/sub.sh'
    subfile = open(subname,'w')
    subfile.write('#!/bin/bash\n')
    subfile.write('cd %s \n' %(jobdir))
    for j in range(1,301): # total 300 files for each energy

        gen_XXX_sh(j,i)
        seqno = str(j)
        subfile.write('chmod 755 run_%s.sh \n' %(seqno))
        subfile.write('hep_sub run_%s.sh -g juno \n' %(seqno))
        subfile.write('\n')


