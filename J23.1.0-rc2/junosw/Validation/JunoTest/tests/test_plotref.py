#!/usr/bin/python

from JunoTest import *

test = UnitTest()

ref = 'hist/histos.root'

test.addCase("histref_cons_kol", "root -q -b run_hist.C", plotRef = ref, histTestMeth = 'Kolmogorov', histTestCut = 0.8)
test.addCase("histref_cons_chi2", "root -q -b run_hist.C", plotRef = ref, histTestMeth = 'Chi2')
test.addCase("histref_incons_kol", "root -q -b run_hist.C", plotRef = ref, histTestMeth = 'Kolmogorov')
test.addCase("histref_incons_chi2", "root -q -b run_hist.C", plotRef = ref, histTestMeth = 'Chi2', histTestCut = 1.0)

test.run()
