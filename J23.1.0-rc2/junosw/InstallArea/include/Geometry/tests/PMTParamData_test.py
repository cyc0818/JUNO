#!/usr/bin/env python
"""
PMTParamData_test.py
====================

::

    In [3]: np.unique(t.pmtCat[:,1], return_counts=True)
    Out[3]: (array([0, 1, 2, 3], dtype=int32), array([ 2720,  4997, 25600,  9895]))

"""

import numpy as np
from opticks.ana.fold import Fold

if __name__ == '__main__':
    t = Fold.Load(symbol="t")
    print(repr(t))
pass

