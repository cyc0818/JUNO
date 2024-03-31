class PlotMem {
  public:

    PlotMem(){ 
      std::cout << "Hello, PlotMem!" << std::endl;
    }

    void setFileName(std::string aString){
      fileName=aString;
    }

    void setOutName(std::string aString){
      outName=aString;
    }

    void setTitle(std::string aString){
      plotTitle=aString;
    }

    void parse_log_mem() {
      std::ifstream ifs(fileName.c_str());

      std::string tmp_line;
      double virt=0; // unit: KB
      double phys=0; // unit: KB
      double time=0;
      times.push_back(time);
      mem_virt.push_back(virt);
      mem_phys.push_back(phys);
      while(ifs.good()) {
        std::getline(ifs, tmp_line);
        std::stringstream ss;
        ss << tmp_line;
        ss >> virt >> phys;
//        std::cout << virt << " " << phys << std::endl; 
        if (ss.fail()) {
          continue;
        }
        time+=5.;
        times.push_back(time);
        mem_virt.push_back(virt);
        mem_phys.push_back(phys);
      }
    }

    void Draw(){
      TCanvas *aCanvas= new TCanvas("Mem_Graphs");
      TGraph *g_virt = new TGraph(mem_virt.size(), &(times[0]),&(mem_virt[0]));
      TGraph *g_phys = new TGraph(mem_phys.size(), &(times[0]),&(mem_phys[0]));
      g_virt->SetTitle(outName.c_str());
      g_virt->GetXaxis()->SetTitle("T[s]");
      g_virt->GetYaxis()->SetTitle("KBytes");
      g_virt->SetTitle(plotTitle.c_str());
      g_virt->Draw();
      g_phys->Draw("same");
      aCanvas->SaveAs((outName+".pdf").c_str());
      aCanvas->SaveAs((outName+".jpg").c_str());
      aCanvas->SaveAs((outName+".svg").c_str());
    }
        
    std::string fileName;
    std::string outName;
    std::string plotTitle;
    std::vector<double> mem_virt;
    std::vector<double> mem_phys;
    std::vector<double> times;

};
