#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

"""
    This script is used to convert the wavelength based Emission Spectrum to 
    energy based Emission Spectrum.
    Let's assume the spectrums are:
    * f(l)dl, l stands for wavelength (nm)
    * g(e)de, e stands for energy (eV)

    The conversion is:
    * l = 1240./e
    * dl/de = -1240/(e**2) = - l**2/1240
    * f(l)dl = f(l(e)) dl/de de
             = f(l(e)) l**2/1240 de
    * g(e) = f(l(e)) l**2/1240

    The l and f(l(e)) is read from the txt file, then the output is 
    * e 
    * g(e)
"""

def load_waveform_based(filename):
    data = []
    with open(filename) as f:
        for line in f:
            line = line.strip()
            if line.startswith("#"): 
                continue

            wavelength, prob = map(float, line.split())
            data.append((wavelength, prob))

    return data

def convert_to_energy_based(data):
    newdata = []
    for wavelength, prob in data:
        energy = 1240./wavelength
        newprob = prob * wavelength**2/1240
        newdata.append((energy, newprob))
    maxval = max(i[1] for i in newdata)
    newdata_norm = [(e, p/maxval) for e,p in newdata]

    return newdata_norm

def formatter_c(data, rev=True, col1name="energy", col1unit="*eV",
                      col2name="prob", col2unit=""):
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
    wave_based_data = load_waveform_based("reemission-20150615.txt")
    energy_based_data = convert_to_energy_based(wave_based_data)

    formatter_c(energy_based_data)

if __name__ == "__main__":
    main()
