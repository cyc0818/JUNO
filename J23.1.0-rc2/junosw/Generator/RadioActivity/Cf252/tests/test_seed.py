"""
EXAMPLE TESTS TO HELP YOU GET STARTED

This "test_seed.py" and "tests" directory was copied here from "seedtests": 
  * http://dayabay.ihep.ac.cn/tracs/dybsvn/browser/dybgaudi/trunk/DybRelease/seedtests/

By the distributor script:
  * http://dayabay.ihep.ac.cn/tracs/dybsvn/browser/dybgaudi/trunk/DybRelease/scripts/distribute_seedtests.py

For help on testing:
  * http://dayabay.ihep.ac.cn/cgi-bin/DocDB/ShowDocument?docid=5258 "Your NuWa Testing System"

And older docs:
  * http://dayabay.ihep.ac.cn/cgi-bin/DocDB/ShowDocument?docid=3645
  * http://dayabay.ihep.ac.cn/cgi-bin/DocDB/ShowDocument?docid=3091

Invoke the tests with:
   nosetests -sv tests/test_seed.py               ## run a single modules tests
   nosetests -sv tests/test_seed.py:test_count    ## run a single test in a single module
   nosetests -sv                                  ## run all tests in all modules

"""

import os
from dybtest import Run, Matcher
checks = {
    '.*FATAL':2,
    '.*\*\*\* Break \*\*\* segmentation violation':3,
    '.*IOError':4,
    '.*ValueError\:':5,
    '^\#\d':None
         }

Run.parser = Matcher( checks, verbose=False )
Run.opts = { 'maxtime':300 }
                      
"""
Check for changes in the help 
"""
def test_nuwahelp():
    Run("nuwa.py --help", reference=True)()

"""
Count the test_*.py modules in the tests directory 
"""
def test_count():
    n = 0 
    for p in os.listdir('tests'):
        if p.startswith('test_') and p.endswith('.py'):
            n += 1 
    assert n > 1 , "ONLY FIND %d TEST MODULE(S) ... YOU DID NOT ADD ANY" % n

"""
Short version of test_count
"""
def test_count_short():
    n = len(filter(lambda p:p.startswith('test_') and p.endswith('.py'), os.listdir('tests'))) 
    assert n > 1 , "ONLY FIND %d TEST MODULE(S) ... YOU DID NOT ADD ANY" % n
test_count_short.__test__ = False


if __name__=='__main__':
    test_nuwahelp()
    test_count()
    test_count_short()


