#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

"""
    This script will load energy(nm) and abs len(cm), and generate a file (c++)

    energy(eV) = 1240./wavelength(nm)

    An example is water_abs_orig.txt
    
    The data in memory is a pair of (e, abs)
"""

def load_energy_based(filename):
    data = []
    with open(filename) as f:
        for line in f:
            line = line.strip()
            # skip the comment line
            if line.startswith("#") or line.startswith("//"):
                continue
            # replace the comma
            line = line.replace(",", "")

            e, abslen = map(float, line.split())
            data.append((e, abslen))
    return data

def formatter_c(data, rev=True, col1name="energy", col1unit="*eV",
                      col2name="abslen", col2unit="*cm"):
    length = len(data)
    # reverse the data
    if rev:
        data = data[::-1]
    print "double %s[%d] = { "%(col1name, length)
    for i in range(length):
        sep = ',' if i != length-1 else ""
        print "%f%s%s"%(data[i][0], col1unit, sep)
    print "};"
    print "double %s[%d] = { "%(col2name, length)
    for i in range(length):
        sep = ',' if i != length-1 else ""
        print "%f%s%s //%f%s"%(data[i][1], col2unit, sep, data[i][0], col1unit)
    print "};"


def main():
    energy_based_data = load_energy_based("water_abs_orig.txt")

    formatter_c(energy_based_data, rev=False)

if __name__ == "__main__":
    main()
