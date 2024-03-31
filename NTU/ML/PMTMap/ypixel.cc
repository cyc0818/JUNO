void ypixel(){

    //TString inpath = "/junofs/users/yuchincheng/NTU/ML/PMTMap/PMTPos_CD_LPMT.csv";
    TString inpath = "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/data/Detector/Geometry/PMTPos_CD_LPMT.csv";
    TString outpath = "/junofs/users/yuchincheng/NTU/ML/PMTMap/ypixel.root";
    string line;
    float z,lastz;
    ifstream infile(inpath);
    TFile* outfile = new TFile(outpath, "recreate");
    TTree* tree = new TTree("YPIXEL", "YPIXEL");
    tree->Branch("z",&z);
    lastz = 0;


    int a = -1;
    while(infile.good()){
        a++;
        
        getline(infile, line);
        stringstream ss(line);
        string word;
        vector<float> position;


        for(int i = 0; i<4; i++){

            ss >> word;
            if(i == 0) continue;
            if(word.empty()) continue;
            position.push_back(stof(word));

        }

        if(position.size() == 0) continue;
        z = position.at(2);
        if(lastz == 0){
            tree->Fill();
            lastz = z;
        }
        else{
            if((int)z != (int)lastz){
                tree->Fill();
                lastz = z;
            }
        }

    }


    tree->Write();
    outfile->Close();


}



