#!/usr/bin/env python


from Tutorial import JUNOApplication
from Tutorial import JUNODBIModule
from Tutorial import JUNOParaDBModule
from Tutorial import JUNOModule

##############################################################################
# Parse arguments
##############################################################################
juno_application = JUNOApplication(description="JUNO ParaDB testing")

dbi_module = JUNODBIModule()
paradb_module = JUNOParaDBModule()

juno_application.register(dbi_module)
juno_application.register(paradb_module)

##############################################################################
# Testing Algorithm
##############################################################################

class DummyParaDBTestModule(JUNOModule):

    def register_options(self, parser):
        pass

    def init(self, toptask, args):
        import ParaDB

        # paradbsvc = toptask.createSvc("ParaDBSvc")
        # paradbsvc.property("RepoNames").set(["DBParaRepo"])
        # paradbsvc.property("RepoTypes").set(["DBI"])
        # paradbsvc.property("ParaGTag").set("J22.1.0")

        alg = toptask.createAlg("TestParaDBAlg")

dbi_test_module = DummyParaDBTestModule()
juno_application.register(dbi_test_module)

juno_application.run()
