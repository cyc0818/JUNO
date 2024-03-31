{
  gROOT->ProcessLine(".L Plots/Scripts/PlotMem.C");

  PlotMem DetSim_1; 
  DetSim_1.setFileName("Log/1_1000_0_20190123.log.mem.usage");
  DetSim_1.setTitle("Memory Vs Time - DetSim - Electrons at Center");
  DetSim_1.setOutName("Plots/1_detsim");
  DetSim_1.parse_log_mem();
  DetSim_1.Draw();

  PlotMem DetSim_2;
  DetSim_2.setFileName("Log/2_1000_0_20190123.log.mem.usage");
  DetSim_2.setTitle("Memory Vs Time - DetSim - Electrons near Top");
  DetSim_2.setOutName("Plots/2_detsim");
  DetSim_2.parse_log_mem();
  DetSim_2.Draw();

  PlotMem DetSim_3;
  DetSim_3.setFileName("Log/3_1000_0_20190123.log.mem.usage");
  DetSim_3.setTitle("Memory Vs Time - DetSim - Positrons 1MeV at Center");
  DetSim_3.setOutName("Plots/3_detsim");
  DetSim_3.parse_log_mem();
  DetSim_3.Draw();

  PlotMem DetSim_4;
  DetSim_4.setFileName("Log/4_1000_0_20190123.log.mem.usage");
  DetSim_4.setTitle("Memory Vs Time - DetSim - Positrons 0-20MeV at Center");
  DetSim_4.setOutName("Plots/4_detsim");
  DetSim_4.parse_log_mem();
  DetSim_4.Draw();

  PlotMem DetSim_5;
  DetSim_5.setFileName("Log/5_1000_0_20190123.log.mem.usage");
  DetSim_5.setTitle("Memory Vs Time - DetSim - Positrons in CD");
  DetSim_5.setOutName("Plots/5_detsim");
  DetSim_5.parse_log_mem();
  DetSim_5.Draw();

  PlotMem DetSim_6;
  DetSim_6.setFileName("Log/6_1000_0_20190123.log.mem.usage");
  DetSim_6.setTitle("Memory Vs Time - DetSim - IBD e+");
  DetSim_6.setOutName("Plots/6_detsim");
  DetSim_6.parse_log_mem();
  DetSim_6.Draw();

  PlotMem DetSim_7;
  DetSim_7.setFileName("Log/7_1000_0_20190123.log.mem.usage");
  DetSim_7.setTitle("Memory Vs Time - DetSim - IBD n");
  DetSim_7.setOutName("Plots/7_detsim");
  DetSim_7.parse_log_mem();
  DetSim_7.Draw();

  PlotMem DetSim_8;
  DetSim_8.setFileName("Log/8_1000_0_20190123.log.mem.usage");
  DetSim_8.setTitle("Memory Vs Time - DetSim - IBD");
  DetSim_8.setOutName("Plots/8_detsim");
  DetSim_8.parse_log_mem();
  DetSim_8.Draw();

  PlotMem DetSim_9_0;
  DetSim_9_0.setFileName("Log/9_100_0_20190123.log.mem.usage");
  DetSim_9_0.setTitle("Memory Vs Time - DetSim - Mu (run 0)");
  DetSim_9_0.setOutName("Plots/9_0_detsim");
  DetSim_9_0.parse_log_mem();
  DetSim_9_0.Draw();

  PlotMem DetSim_9_1;
  DetSim_9_1.setFileName("Log/9_100_1_20190123.log.mem.usage");
  DetSim_9_1.setTitle("Memory Vs Time - DetSim - Mu (run 1)");
  DetSim_9_1.setOutName("Plots/9_1_detsim");
  DetSim_9_1.parse_log_mem();
  DetSim_9_1.Draw();

  PlotMem DetSim_9_2;
  DetSim_9_2.setFileName("Log/9_100_2_20190123.log.mem.usage");
  DetSim_9_2.setTitle("Memory Vs Time - DetSim - Mu (run 2)");
  DetSim_9_2.setOutName("Plots/9_2_detsim");
  DetSim_9_2.parse_log_mem();
  DetSim_9_2.Draw();

  PlotMem DetSim_9_3;
  DetSim_9_3.setFileName("Log/9_100_3_20190123.log.mem.usage");
  DetSim_9_3.setTitle("Memory Vs Time - DetSim - Mu (run 3)");
  DetSim_9_3.setOutName("Plots/9_3_detsim");
  DetSim_9_3.parse_log_mem();
  DetSim_9_3.Draw();

  PlotMem DetSim_9_4;
  DetSim_9_4.setFileName("Log/9_100_4_20190123.log.mem.usage");
  DetSim_9_4.setTitle("Memory Vs Time - DetSim - Mu (run 4)");
  DetSim_9_4.setOutName("Plots/9_4_detsim");
  DetSim_9_4.parse_log_mem();
  DetSim_9_4.Draw();

  PlotMem DetSim_9_5;
  DetSim_9_5.setFileName("Log/9_100_5_20190123.log.mem.usage");
  DetSim_9_5.setTitle("Memory Vs Time - DetSim - Mu (run 5)");
  DetSim_9_5.setOutName("Plots/9_5_detsim");
  DetSim_9_5.parse_log_mem();
  DetSim_9_5.Draw();

  PlotMem DetSim_9_6;
  DetSim_9_6.setFileName("Log/9_100_6_20190123.log.mem.usage");
  DetSim_9_6.setTitle("Memory Vs Time - DetSim - Mu (run 6)");
  DetSim_9_6.setOutName("Plots/9_6_detsim");
  DetSim_9_6.parse_log_mem();
  DetSim_9_6.Draw();

  PlotMem DetSim_9_7;
  DetSim_9_7.setFileName("Log/9_100_7_20190123.log.mem.usage");
  DetSim_9_7.setTitle("Memory Vs Time - DetSim - Mu (run 7)");
  DetSim_9_7.setOutName("Plots/9_7_detsim");
  DetSim_9_7.parse_log_mem();
  DetSim_9_7.Draw();

  PlotMem DetSim_9_8;
  DetSim_9_8.setFileName("Log/9_100_8_20190123.log.mem.usage");
  DetSim_9_8.setTitle("Memory Vs Time - DetSim - Mu (run 8)");
  DetSim_9_8.setOutName("Plots/9_8_detsim");
  DetSim_9_8.parse_log_mem();
  DetSim_9_8.Draw();

  PlotMem DetSim_9_9;
  DetSim_9_9.setFileName("Log/9_100_9_20190123.log.mem.usage");
  DetSim_9_9.setTitle("Memory Vs Time - DetSim - Mu (run 9)");
  DetSim_9_9.setOutName("Plots/9_9_detsim");
  DetSim_9_9.parse_log_mem();
  DetSim_9_9.Draw();
  
  PlotMem ElecSim_1;
  ElecSim_1.setFileName("Elec/electron_20190123_0_1000_1MeV_Center_output/electron_20190123_0_1000_1MeV_Center_output.log.mem.usage");
  ElecSim_1.setTitle("Memory Vs Time - ElecSim - Electrons at Center");
  ElecSim_1.setOutName("Plots/1_elecsim");
  ElecSim_1.parse_log_mem();
  ElecSim_1.Draw();

  PlotMem ElecSim_2;
  ElecSim_2.setFileName("Elec/electron_20190123_0_1000_1MeV_Top_output/electron_20190123_0_1000_1MeV_Top_output.log.mem.usage");
  ElecSim_2.setTitle("Memory Vs Time - ElecSim - Electrons near Top ");
  ElecSim_2.setOutName("Plots/2_elecsim");
  ElecSim_2.parse_log_mem();
  ElecSim_2.Draw();

  PlotMem ElecSim_3;
  ElecSim_3.setFileName("Elec/positrons_20190123_0_1000_1MeV_output/positrons_20190123_0_1000_1MeV_output.log.mem.usage");
  ElecSim_3.setTitle("Memory Vs Time - ElecSim - Positrons 1MeV at Center");
  ElecSim_3.setOutName("Plots/3_elecsim");
  ElecSim_3.parse_log_mem();
  ElecSim_3.Draw();

  PlotMem ElecSim_4;
  ElecSim_4.setFileName("Elec/positrons_20190123_0_1000_To20MeV_output/positrons_20190123_0_1000_To20MeV_output.log.mem.usage");
  ElecSim_4.setTitle("Memory Vs Time - ElecSim - Positrons 0-20MeV at Center");
  ElecSim_4.setOutName("Plots/4_elecsim");
  ElecSim_4.parse_log_mem();
  ElecSim_4.Draw();

  PlotMem ElecSim_5;
  ElecSim_5.setFileName("Elec/positrons_20190123_0_1000_CDTo20MeV_output/positrons_20190123_0_1000_CDTo20MeV_output.log.mem.usage");
  ElecSim_5.setTitle("Memory Vs Time - ElecSim - Positrons in CD");
  ElecSim_5.setOutName("Plots/5_elecsim");
  ElecSim_5.parse_log_mem();
  ElecSim_5.Draw();

  PlotMem ElecSim_6;
  ElecSim_6.setFileName("Elec/IBD-eplus_20190123_0_1000_output/IBD-eplus_20190123_0_1000_output.log.mem.usage");
  ElecSim_6.setTitle("Memory Vs Time - ElecSim - IBD e+");
  ElecSim_6.setOutName("Plots/6_elecsim");
  ElecSim_6.parse_log_mem();
  ElecSim_6.Draw();

  PlotMem ElecSim_7;
  ElecSim_7.setFileName("Elec/IBD-neutron_20190123_0_1000_output/IBD-neutron_20190123_0_1000_output.log.mem.usage");
  ElecSim_7.setTitle("Memory Vs Time - ElecSim - IBD n");
  ElecSim_7.setOutName("Plots/7_elecsim");
  ElecSim_7.parse_log_mem();
  ElecSim_7.Draw();

  PlotMem ElecSim_8;
  ElecSim_8.setFileName("Elec/IBD_20190123_0_1000_output/IBD_20190123_0_1000_output.log.mem.usage");
  ElecSim_8.setTitle("Memory Vs Time - ElecSim - IBD");
  ElecSim_8.setOutName("Plots/8_elecsim");
  ElecSim_8.parse_log_mem();
  ElecSim_8.Draw();

  PlotMem ElecSim_9_0;
  ElecSim_9_0.setFileName("Elec/Mu_20190123_0_100_output/Mu_20190123_0_100_output.log.mem.usage");
  ElecSim_9_0.setTitle("Memory Vs Time - ElecSim - Mu (run 0)");
  ElecSim_9_0.setOutName("Plots/9_0_elecsim");
  ElecSim_9_0.parse_log_mem();
  ElecSim_9_0.Draw();

  PlotMem ElecSim_9_1;
  ElecSim_9_1.setFileName("Elec/Mu_20190123_1_100_output/Mu_20190123_1_100_output.log.mem.usage");
  ElecSim_9_1.setTitle("Memory Vs Time - ElecSim - Mu (run 1)");
  ElecSim_9_1.setOutName("Plots/9_1_elecsim");
  ElecSim_9_1.parse_log_mem();
  ElecSim_9_1.Draw();

  PlotMem ElecSim_9_2;
  ElecSim_9_2.setFileName("Elec/Mu_20190123_2_100_output/Mu_20190123_2_100_output.log.mem.usage");
  ElecSim_9_2.setTitle("Memory Vs Time - ElecSim - Mu (run 2)");
  ElecSim_9_2.setOutName("Plots/9_2_elecsim");
  ElecSim_9_2.parse_log_mem();
  ElecSim_9_2.Draw();

  PlotMem ElecSim_9_3;
  ElecSim_9_3.setFileName("Elec/Mu_20190123_3_100_output/Mu_20190123_3_100_output.log.mem.usage");
  ElecSim_9_3.setTitle("Memory Vs Time - ElecSim - Mu (run 3)");
  ElecSim_9_3.setOutName("Plots/9_3_elecsim");
  ElecSim_9_3.parse_log_mem();
  ElecSim_9_3.Draw();

  PlotMem ElecSim_9_4;
  ElecSim_9_4.setFileName("Elec/Mu_20190123_4_100_output/Mu_20190123_4_100_output.log.mem.usage");
  ElecSim_9_4.setTitle("Memory Vs Time - ElecSim - Mu (run 4)");
  ElecSim_9_4.setOutName("Plots/9_4_elecsim");
  ElecSim_9_4.parse_log_mem();
  ElecSim_9_4.Draw();

  PlotMem ElecSim_9_5;
  ElecSim_9_5.setFileName("Elec/Mu_20190123_5_100_output/Mu_20190123_5_100_output.log.mem.usage");
  ElecSim_9_5.setTitle("Memory Vs Time - ElecSim - Mu (run 05");
  ElecSim_9_5.setOutName("Plots/9_5_elecsim");
  ElecSim_9_5.parse_log_mem();
  ElecSim_9_5.Draw();

  PlotMem ElecSim_9_6;
  ElecSim_9_6.setFileName("Elec/Mu_20190123_6_100_output/Mu_20190123_6_100_output.log.mem.usage");
  ElecSim_9_6.setTitle("Memory Vs Time - ElecSim - Mu (run 6)");
  ElecSim_9_6.setOutName("Plots/9_6_elecsim");
  ElecSim_9_6.parse_log_mem();
  ElecSim_9_6.Draw();

  PlotMem ElecSim_9_7;
  ElecSim_9_7.setFileName("Elec/Mu_20190123_7_100_output/Mu_20190123_7_100_output.log.mem.usage");
  ElecSim_9_7.setTitle("Memory Vs Time - ElecSim - Mu (run 7)");
  ElecSim_9_7.setOutName("Plots/9_7_elecsim");
  ElecSim_9_7.parse_log_mem();
  ElecSim_9_7.Draw();

  PlotMem ElecSim_9_8;
  ElecSim_9_8.setFileName("Elec/Mu_20190123_8_100_output/Mu_20190123_8_100_output.log.mem.usage");
  ElecSim_9_8.setTitle("Memory Vs Time - ElecSim - Mu (run 8)");
  ElecSim_9_8.setOutName("Plots/9_8_elecsim");
  ElecSim_9_8.parse_log_mem();
  ElecSim_9_8.Draw();

  PlotMem ElecSim_9_9;
  ElecSim_9_9.setFileName("Elec/Mu_20190123_9_100_output/Mu_20190123_9_100_output.log.mem.usage");
  ElecSim_9_9.setTitle("Memory Vs Time - ElecSim - Mu (run 9)");
  ElecSim_9_9.setOutName("Plots/9_9_elecsim");
  ElecSim_9_9.parse_log_mem();
  ElecSim_9_9.Draw();

}
