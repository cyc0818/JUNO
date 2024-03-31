#!/usr/bin/env python

from Tutorial import JUNOApplication
from Tutorial import JUNODBIModule
from Tutorial import JUNOModule

##############################################################################
# Parse arguments
##############################################################################
juno_application = JUNOApplication(description="JUNO DBISvc testing")

dbi_module = JUNODBIModule()

juno_application.register(dbi_module)

##############################################################################
# Testing Algorithm
##############################################################################

class DummyDBITestModule(JUNOModule):

    def register_options(self, parser):
        pass

    def init(self, toptask, args):
        import DBISvc
        alg = toptask.createAlg("DummyDBISvcTestAlg")

dbi_test_module = DummyDBITestModule()
juno_application.register(dbi_test_module)

juno_application.run()
