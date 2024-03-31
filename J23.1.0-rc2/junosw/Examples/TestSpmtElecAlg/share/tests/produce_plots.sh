#!/bin/bash

echo "Producing Plots..."
echo "tag: " $1

python ../TestSpmtElec.py ROOT/electron_$1_0_1000_1MeV_Center_output.root Plots/RootFiles/Plots_DetSim_Electrons_1MeV_Center.root
cd Plots
root -b -q Scripts/Histo1_Electrons_1MeV_Center.C
cd ..

python ../TestSpmtElec.py Elec/electron_$1_0_1000_1MeV_Center_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_Electrons_1MeV_Center.root
cd Plots
root -b -q Scripts/Histo2_Electrons_1MeV_Center.C
root -b -q Scripts/Histo3_Electrons_1MeV_Center.C
root -b -q Scripts/Histo4_Electrons_1MeV_Center.C

cd ..

python ../TestSpmtElec.py ROOT/electron_$1_0_1000_1MeV_Top_output.root Plots/RootFiles/Plots_DetSim_Electrons_1MeV_Top.root
cd Plots
root -b -q Scripts/Histo1_Electrons_1MeV_Top.C
cd ..

python ../TestSpmtElec.py Elec/electron_$1_0_1000_1MeV_Top_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_Electrons_1MeV_Top.root
cd Plots
root -b -q Scripts/Histo2_Electrons_1MeV_Top.C
root -b -q Scripts/Histo3_Electrons_1MeV_Top.C
root -b -q Scripts/Histo4_Electrons_1MeV_Top.C
cd ..

python ../TestSpmtElec.py ROOT/positrons_$1_0_1000_1MeV_output.root Plots/RootFiles/Plots_DetSim_Positrons_1MeV_Center.root
cd Plots
root -b -q Scripts/Histo1_Positrons_1MeV_Center.C
cd ..

python ../TestSpmtElec.py Elec/positrons_$1_0_1000_1MeV_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_Positrons_1MeV_Center.root
cd Plots
root -b -q Scripts/Histo2_Positrons_1MeV_Center.C
root -b -q Scripts/Histo3_Positrons_1MeV_Center.C
root -b -q Scripts/Histo4_Positrons_1MeV_Center.C
cd ..

python ../TestSpmtElec.py ROOT/positrons_$1_0_1000_To20MeV_output.root Plots/RootFiles/Plots_DetSim_Positrons_To20MeV_Center.root
cd Plots
root -b -q Scripts/Histo1_Positrons_To20MeV_Center.C
cd ..

python ../TestSpmtElec.py Elec/positrons_$1_0_1000_To20MeV_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_Positrons_To20MeV_Center.root
cd Plots
root -b -q Scripts/Histo2_Positrons_To20MeV_Center.C
root -b -q Scripts/Histo3_Positrons_To20MeV_Center.C
root -b -q Scripts/Histo4_Positrons_To20MeV_Center.C
cd ..

python ../TestSpmtElec.py ROOT/positrons_$1_0_1000_CDTo20MeV_output.root Plots/RootFiles/Plots_DetSim_Positrons_To20MeV_CD.root
cd Plots
root -b -q Scripts/Histo1_Positrons_To20MeV_CD.C
cd ..

python ../TestSpmtElec.py Elec/positrons_$1_0_1000_CDTo20MeV_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_Positrons_To20MeV_CD.root
cd Plots
root -b -q Scripts/Histo2_Positrons_To20MeV_CD.C
root -b -q Scripts/Histo3_Positrons_To20MeV_CD.C
root -b -q Scripts/Histo4_Positrons_To20MeV_CD.C
cd ..

python ../TestSpmtElec.py ROOT/IBD-eplus_$1_0_1000_output.root Plots/RootFiles/Plots_DetSim_IBD_e.root
cd Plots
root -b -q Scripts/Histo1_IBD_e.C
cd ..

python ../TestSpmtElec.py Elec/IBD-eplus_$1_0_1000_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_IBD_e.root
cd Plots
root -b -q Scripts/Histo2_IBD_e.C
root -b -q Scripts/Histo3_IBD_e.C
root -b -q Scripts/Histo4_IBD_e.C
cd ..

python ../TestSpmtElec.py ROOT/IBD-neutron_$1_0_1000_output.root Plots/RootFiles/Plots_DetSim_IBD_n.root
cd Plots
root -b -q Scripts/Histo1_IBD_n.C
cd ..

python ../TestSpmtElec.py Elec/IBD-neutron_$1_0_1000_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_IBD_n.root
cd Plots
root -b -q Scripts/Histo2_IBD_n.C
root -b -q Scripts/Histo3_IBD_n.C
root -b -q Scripts/Histo4_IBD_n.C
cd ..

python ../TestSpmtElec.py ROOT/IBD_$1_0_1000_output.root Plots/RootFiles/Plots_DetSim_IBD.root
cd Plots
root -b -q Scripts/Histo1_IBD.C
cd ..

python ../TestSpmtElec.py Elec/IBD_$1_0_1000_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_IBD.root
cd Plots
root -b -q Scripts/Histo2_IBD.C
root -b -q Scripts/Histo3_IBD.C
root -b -q Scripts/Histo4_IBD.C
cd ..

python ../TestSpmtElec.py ROOT/Mu_$1_0_100_output.root Plots/RootFiles/Plots_DetSim_Mu_0.root
python ../TestSpmtElec.py ROOT/Mu_$1_1_100_output.root Plots/RootFiles/Plots_DetSim_Mu_1.root
python ../TestSpmtElec.py ROOT/Mu_$1_2_100_output.root Plots/RootFiles/Plots_DetSim_Mu_2.root
python ../TestSpmtElec.py ROOT/Mu_$1_3_100_output.root Plots/RootFiles/Plots_DetSim_Mu_3.root
python ../TestSpmtElec.py ROOT/Mu_$1_4_100_output.root Plots/RootFiles/Plots_DetSim_Mu_4.root
python ../TestSpmtElec.py ROOT/Mu_$1_5_100_output.root Plots/RootFiles/Plots_DetSim_Mu_5.root
python ../TestSpmtElec.py ROOT/Mu_$1_6_100_output.root Plots/RootFiles/Plots_DetSim_Mu_6.root
python ../TestSpmtElec.py ROOT/Mu_$1_7_100_output.root Plots/RootFiles/Plots_DetSim_Mu_7.root
python ../TestSpmtElec.py ROOT/Mu_$1_8_100_output.root Plots/RootFiles/Plots_DetSim_Mu_8.root
python ../TestSpmtElec.py ROOT/Mu_$1_9_100_output.root Plots/RootFiles/Plots_DetSim_Mu_9.root
cd Plots
root -b -q Scripts/Histo1_Mu.C
cd ..

python ../TestSpmtElec.py Elec/Mu_$1_0_100_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_Mu_0.root
python ../TestSpmtElec.py Elec/Mu_$1_1_100_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_Mu_1.root
python ../TestSpmtElec.py Elec/Mu_$1_2_100_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_Mu_2.root
python ../TestSpmtElec.py Elec/Mu_$1_3_100_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_Mu_3.root
python ../TestSpmtElec.py Elec/Mu_$1_4_100_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_Mu_4.root
python ../TestSpmtElec.py Elec/Mu_$1_5_100_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_Mu_5.root
python ../TestSpmtElec.py Elec/Mu_$1_6_100_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_Mu_6.root
python ../TestSpmtElec.py Elec/Mu_$1_7_100_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_Mu_7.root
python ../TestSpmtElec.py Elec/Mu_$1_8_100_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_Mu_8.root
python ../TestSpmtElec.py Elec/Mu_$1_9_100_output/sample_elecsim.root Plots/RootFiles/Plots_ElecSim_Mu_9.root
cd Plots
root -b -q Scripts/Histo2_Mu.C
root -b -q Scripts/Histo3_Mu.C
root -b -q Scripts/Histo4_Mu.C
cd ..

