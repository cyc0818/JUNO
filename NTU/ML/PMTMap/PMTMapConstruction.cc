void PMTMapConstruction(){

    TString inpath = "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/data/Detector/Geometry/PMTPos_CD_LPMT.csv";
    TString outpath = "/junofs/users/yuchincheng/NTU/ML/PMTMap/CD_LPMT.root";
    string line;
    double x,y,z;
    ifstream infile(inpath);
    TFile* outfile = new TFile(outpath, "recreate");
    TTree* tree = new TTree("PMTMap", "PMT Position");
    tree->Branch("x",&x);
    tree->Branch("y",&y);
    tree->Branch("z",&z);


    while(infile.good()){
        
        getline(infile, line);
        stringstream ss(line);
        string word;
        vector<double> position;


        for(int i = 0; i<4; i++){

            ss >> word;
            if(i == 0) continue;
            if(word.empty()) continue;
            position.push_back(stod(word));

        }

        if(position.size() == 0) continue;
        x = position.at(0);
        y = position.at(1);
        z = position.at(2);
        tree->Fill();

    }


    tree->Write();
    outfile->Close();


}



