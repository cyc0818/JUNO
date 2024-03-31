import time
import sys

fatalList = [
              'ERROR: this is a dummy error',
              'FATAL: this is a dummy fatal',
              '*** Break *** segmentation violation',
              'Segmentation fault',
              'ImportError: python import failure',
              'TypeError: python type error',
            ]

def makeFatal(n):
  return fatalList[n]

if __name__ == "__main__":
  time.sleep(1)
  print '5'
  print '4'
  sys.stdout.flush()
  time.sleep(2)
  print '3'
  time.sleep(1)
  print '2'
  time.sleep(1)
  print '1'
  sys.stdout.flush()
  print makeFatal(int(sys.argv[1]))
  time.sleep(2)
  print '0'
