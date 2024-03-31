# This Makefile will be maintained manually.
# Maintainer: Tao Lin <lintao@ihep.ac.cn>
#
# + all
#   + edm-html (Event Data Model)
#   + detsim-html (Detector Simulation)
#   + elecsim-html (Electronics Simulation)
all: edm-html 
all: detsim-html

#####################################################################
# Event Data Model
#####################################################################
edm-html: datamodel/simevent.html

#####################################################################
# Detector Simulation
#####################################################################
detsim-html: detsim/index.html
detsim-html: detsim/preq_processing.html
detsim-html: detsim/preq_examples.html
detsim-html: detsim/quickstart.html
detsim-html: detsim/quickstart2.html
detsim-html: detsim/quickstart3.html
detsim-html: detsim/structures.html
detsim-html: detsim/framework.html
detsim-html: detsim/geomelem.html
detsim-html: detsim/material.html
detsim-html: detsim/anaelement.html
detsim-html: detsim/PMTOPmodel.html
detsim-html: detsim/PMTconstruct.html
detsim-html: detsim/appendix_tools.html

# override normal rule, don't generate TOC
%index.html: %index.md
	pandoc -o $@ $^ --self-contained -c j.css

%.html: %.md
	pandoc -o $@ $^ --self-contained --toc -c j.css

.PHONY: 
