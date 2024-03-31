#!/usr/bin/env python

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("output_file")
parser.add_argument("--telescope", action="store_true")
args = parser.parse_args()

fout = open(args.output_file, "w")

# Reference for numerical values in DetSimV2/TopTracker/src/TopTrackerConstruction.cc (r3779)
# all values in mm

wall_side           = 6861.2
wall_overlap_length =  150

zlowerwall          = 1645
zspace              = 1500
zshift              =   50

zlowerwallChimney   = 6215 + zlowerwall
zspaceChimney       =  250

n_row    = 7
n_column = 3
n_layer  = 3

middle_row    = int(n_row/2)
middle_column = int(n_column/2)

fout.write("####################################################################\n")
fout.write("# Positions defined by placement on grid, no survey done.          #\n")
fout.write("#                                                                  #\n")
fout.write("# Field order:                                                     #\n")
fout.write("# <wall ID> <x> <y> <z> <rot angle> <panels>                       #\n")
fout.write("#                                                                  #\n")
fout.write("# * positions are given in mm and angles in degrees                #\n")
fout.write("# * panels corresponds to which panels are in the detector         #\n")
fout.write("#   - for all panels: 0xFF                                         #\n")
fout.write("#   - if panel of PMTs 0 & 11 is missing: 0xFE                     #\n")
fout.write("#   - if panel of PMTs 4 & 15 is missing: 0xEF                     #\n")
fout.write("#                                                                  #\n")
fout.write("# NB: lines starting with # are comments, empty lines are ignored. #\n")
fout.write("####################################################################\n")

#### TT positioning
fout.write("\n")
fout.write("# TT positioning\n")
fout.write("\n")

angle = 90

first_char = " "
if args.telescope:
    first_char = "#"

# NB Loop order is made to produce sorted output, not to optimize calculations
for layer in range(n_layer):
    for column in range(n_column):
        for row in range(n_row):
            y = (column - middle_column) * ( wall_side - wall_overlap_length )
            x =-(row    - middle_row   ) * ( wall_side - wall_overlap_length )
            z = zlowerwall + layer * zspace
            if column == 1 and row == 3:
                # Chimney modules
                z = zlowerwallChimney + layer * zspaceChimney
            elif (column%2 == 1) and (row%2 == 0):
                pass
            elif (column%2 == 0) and (row%2 == 1):
                z += zshift
            elif (column%2 == 0) and (row%2 == 0):
                z += 2 * zshift
            elif (column%2 == 1) and (row%2 == 1):
                z += 3 * zshift

            panels = 0xFF
            module = (layer<<6) + (column<<3) + row

            fout.write(f"{first_char}{module:3d} {x:9.1f} {y:9.1f} {z:9.1f} {angle:3.0f} 0x{panels:X}\n")

#### Telescope
fout.write("\n")
fout.write("# Telescope @ IPHC positioning\n")
fout.write("\n")

angle = 0

distances_from_bottom = [ 0, 200, 800, 1000 ]

first_char = "#"
if args.telescope:
    first_char = " "

for layer in range(len(distances_from_bottom)):
    for column in [7]:
        for row in [7]:
            x = - middle_column * ( wall_side - wall_overlap_length )
            y = - middle_row    * ( wall_side - wall_overlap_length )
            z = zlowerwall + distances_from_bottom[layer]

            panels = 0x11
            module = (layer<<6) + (column<<3) + row

            fout.write(f"{first_char}{module:3d} {x:9.1f} {y:9.1f} {z:9.1f} {angle:3.0f} 0x{panels:X}\n")

fout.close()

## Detector/Parameter/data/Dimensions.txt
