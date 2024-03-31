#!/usr/bin/env python
#-*- coding: utf-8 -*-
#-*- author: Tao Lin -*-
#
# Description: generate the input lists for event mixing. 
#

import argparse
import os.path
import re

class DataPreparation(object):

    def __init__(self):
        self.infos = []
        self.datatop = ""

    def parse_args(self):
        self.parser = argparse.ArgumentParser(description="Prepare input lists")
        self.parser.add_argument("--yaml", help="the YAML file")
        self.parser.add_argument("--datatop", help="find the input files under this directory")

        self.args = self.parser.parse_args()

    def run(self):

        self.parse_args()

        if self.args.yaml:
            self.load_yaml(self.args.yaml)

        if self.args.datatop:
            self.setup_datatop(self.args.datatop)

        self.create_filelists()

    def load_yaml(self, fn):
        import yaml

        with open(fn) as f:
            self.infos = yaml.safe_load(f)

    def setup_datatop(self, datadir):
        self.datatop = datadir

    def create_filelists(self):
        for info in self.infos:
            label = info["label"]
            inputitem = info["input"]

            # skip if it is not a filelist starting with @
            if not inputitem.startswith("@"):
                continue
            
            inputitem = inputitem[1:]

            self.create_filelist(label, inputitem)

    def create_filelist(self, label, outputfn):

        with open(outputfn, "w") as outf:
            for f in self.get_filelist(label):
                outf.write(f)
                outf.write("\n")

    def get_filelist(self, label):
        # assume the data directory:
        #   datatop/U238/U238-LS/Single/detsim/group1/detsim-10.root
        #             ^     ^
        #             |     |
        #          Nuclide  |
        #                Material

        nuclide = label.split('-')[0]
        material = label
        tag = "Single"
        stage = "detsim"
        group_pattern = re.compile(r'group\d+')
        file_pattern = re.compile(r'detsim-\d+.root')

        d = os.path.join(self.datatop, 
                         nuclide, 
                         material,
                         tag,
                         stage)

        for group in os.listdir(d):

            if not group_pattern.match(group):
                continue

            dd = os.path.join(d, group)

            for fn in os.listdir(dd):
                if not file_pattern.match(fn):
                    continue

                f = os.path.join(dd, fn)
                yield f


if __name__ == "__main__":
    dp = DataPreparation()

    dp.run()
