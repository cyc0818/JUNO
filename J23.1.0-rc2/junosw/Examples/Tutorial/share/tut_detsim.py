#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os, logging
log = logging.getLogger(__name__)
level = os.environ.get("LOGLEVEL", "info")
fmt = '[%(asctime)s] p%(process)s {%(pathname)s:%(lineno)d} %(levelname)s - %(message)s'
logging.basicConfig(level=getattr(logging, level.upper()), format=fmt)

from Tutorial import JUNOApplication
from Tutorial import JUNODetSimModule

juno_application = JUNOApplication(description="JUNO Generator && Detector Simulation")
juno_application.set_default_output("sample_detsim.root")
juno_application.set_default_user_output("sample_detsim_user.root")

detsim_module = JUNODetSimModule()

juno_application.register(detsim_module)
log.info("after juno_application.register(detsim_module) ")
juno_application.run()

