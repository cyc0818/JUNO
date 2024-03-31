#!/usr/bin/env python
'''
This fetches ENSDF files from ie.lbl.gov for libmore.
Set MORE_PHYS_FETCHER to point to this script.
'''

import sys, os
try:
    nuc = sys.argv[1]
    target_dir = sys.argv[2]
except IndexError:
    print('''
Usage: lbl-ensdf-fetcher.py NUC local_file_path 
       NUC is [Abreviated name]-[mass number]
       If using with libmore, set MORE_PHYS_FETCHER to point to me
''')

override = os.getenv('MORE_ENSDF_DIR',None)
if override:
    target_dir = override

name,A = nuc.split('-')
#filename = '%(a)03d%(n)s.ens'%{'n':name,'a':int(A)}
filename = 'ar%03d%s.ens.bz2' %( int(A), name.lower() )

path = os.path.join(target_dir,filename)

if os.path.exists(path):
    sys.exit(0)
print('No such file: %s' % path)
sys.exit(0)

