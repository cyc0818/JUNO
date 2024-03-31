#!/usr/bin/env python
#-*- coding: utf-8 -*-
#-*- author: Tao Lin -*-
#
# Description: calculate the total rate
#

import argparse
import os.path
import re

class RateCalculator(object):

    def __init__(self):
        self.infos = []

    def parse_args(self):
        self.parser = argparse.ArgumentParser(description="Prepare input lists")
        self.parser.add_argument("--yaml", help="the YAML file")

        self.args = self.parser.parse_args()

    def run(self):

        self.parse_args()

        if self.args.yaml:
            self.load_yaml(self.args.yaml)

        self.calculate()

    def load_yaml(self, fn):
        import yaml

        with open(fn) as f:
            self.infos = yaml.safe_load(f)

    def calculate(self):
        totalrates = 0.0
        for info in self.infos:
            rate = float(info["rate"])

            totalrates += rate

        print(f"Total rates: {totalrates}")

if __name__ == "__main__":
    rc = RateCalculator()

    rc.run()

