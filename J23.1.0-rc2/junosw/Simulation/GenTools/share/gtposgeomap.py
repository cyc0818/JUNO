#!/usr/bin/env python
#-*- coding: utf-8 -*-
#-*- author: Tao Lin <lintao AT ihep.ac.cn> -*-

import os
import yaml

def load_config(config_file):
    with open(config_file) as f:
        return yaml.safe_load(f)


DATA_MATERIALS = {"PMT_20inch_body_phys": "Pyrex",
                  "PMT_3inch_body_phys": "Pyrex",
                  "pCentralDetector": "Steel",
                  "pTarget": "LS",
                  "pSource": "Analcime",
                  "pGT_Source": {"pSourceEnclosure_cavity": "VacuumT"},
                  "pTopRock": "Rock",
                  "pBtmRock": "Rock",
                  "pBar": "Scintillator",
                  "LatticedShell": {"pBtmRock": "LatticedShellSteel"},
                  # for TWO-mask mode. note: PMT_20inch_body_phys is for WP.
                  "pLPMT_Hamamatsu_R12860": "Pyrex",
                  "pLPMT_NNVT_MCPPMT": "Pyrex",
}


# dump DATA_MATERIALS to yaml
print(yaml.dump(DATA_MATERIALS))


conf = os.environ.get("TUTORIALROOT")
if conf:
    conf = os.path.join(conf, "share/mac/GtPosGeoMapping.yaml")

if os.path.exists(conf):
    geomat_conf = load_config(conf)
    print(geomat_conf)

    n = "LPMT_CD"
    if n in geomat_conf:
        print([k for k in geomat_conf[n]])
