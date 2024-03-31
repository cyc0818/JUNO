void Check(){

    TString inpath = "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/data/Detector/Geometry/PMTPos_CD_SPMT.csv";
    TString outpath = "/junofs/users/yuchincheng/NTU/ML/PMTMap/CD_SPMT.root";
    string line;
    double x1,y1,z1,x2,y2,z2;
    ifstream infile(inpath);
    TFile* outfile = new TFile(outpath);
    TTree* tree = outfile->Get<TTree>("PMTMap");
    tree->SetBranchAddress("x",&x2);
    tree->SetBranchAddress("y",&y2);
    tree->SetBranchAddress("z",&z2);


    int a = -1;
    while(infile.good()){
        
        a++;
        getline(infile, line);
        stringstream ss(line);
        string word;
        vector<double> position;


        for(int i = 0; i<4; i++){

            getline(ss, word, ' ');
            if(i == 0) continue;
            if(word.empty()) continue;
            position.push_back(stod(word));

        }

        if(position.size() == 0) continue;
        x1 = position.at(0);
        y1 = position.at(1);
        z1 = position.at(2);
        tree->GetEntry(a);
        cout << line << endl;
        if(x1!=x2 or y1!=y2 or z1!=z2) cout << "Different channel = " << a << endl;

    }


    outfile->Close();


}



