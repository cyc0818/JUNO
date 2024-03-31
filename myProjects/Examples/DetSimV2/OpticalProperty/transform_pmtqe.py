#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

"""
    This script will load wavelength(nm) and QE, and generate a file (c++)
    containing energy(eV) vs QE

    energy(eV) = 1240./wavelength(nm)

    An example is pmt_qe_1inch_20140620.txt
    
    The data in memory is a pair of (l, qe)
"""

def load_waveform_based(filename):
    data = []
    with open(filename) as f:
        for line in f:
            line = line.strip()
            # skip the comment line
            if line.startswith("#") or line.startswith("//"):
                continue
            # replace the comma
            line = line.replace(",", "")

            wavelength, qe = map(float, line.split())
            data.append((wavelength, qe/100.))
    return data

def convert_to_energy_based(data):
    newdata = [(1240./l, qe) for l, qe in data]

    return newdata

def formatter_c(data, rev=True, col1name="energy", col1unit="*eV",
                      col2name="qe", col2unit=""):
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
    wave_based_data = load_waveform_based("pmt_qe_1inch_20140620.txt")
    energy_based_data = convert_to_energy_based(wave_based_data)

    formatter_c(energy_based_data)

if __name__ == "__main__":
    main()
