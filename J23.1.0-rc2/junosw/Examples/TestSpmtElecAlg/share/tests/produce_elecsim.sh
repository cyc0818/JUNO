#!/bin/bash

# $1 : tag
# $2 : setup script

./submit_elec.sh electron_$1_0_1000_1MeV_Center_output $2
./submit_elec.sh electron_$1_0_1000_1MeV_Top_output $2
./submit_elec.sh IBD_$1_0_1000_output $2
./submit_elec.sh IBD-eplus_$1_0_1000_output $2
./submit_elec.sh IBD-neutron_$1_0_1000_output $2
./submit_elec.sh Mu_$1_0_100_output $2
./submit_elec.sh Mu_$1_1_100_output $2
./submit_elec.sh Mu_$1_2_100_output $2
./submit_elec.sh Mu_$1_3_100_output $2
./submit_elec.sh Mu_$1_4_100_output $2
./submit_elec.sh Mu_$1_5_100_output $2
./submit_elec.sh Mu_$1_6_100_output $2
./submit_elec.sh Mu_$1_7_100_output $2
./submit_elec.sh Mu_$1_8_100_output $2
./submit_elec.sh Mu_$1_9_100_output $2
./submit_elec.sh positrons_$1_0_1000_1MeV_output $2
./submit_elec.sh positrons_$1_0_1000_CDTo20MeV_output $2
./submit_elec.sh positrons_$1_0_1000_To20MeV_output $2
