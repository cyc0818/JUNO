#include <fstream>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "PMTinfo.h"


int main(int argc, char** argv){

    if(argc != 3){
        cerr << "Two arguments are required!" << endl;
    }


    const double shift = -280;
    const double normalization = 1./(1800.-shift);


    // Read position of each LPMT
    vector<PMTinfo> lpmts, spmts;
    double x, y, z;
    TFile* LPMTFile = new TFile("/junofs/users/yuchincheng/NTU/ML/PMTMap/CD_LPMT.root");
    TTree* LPMTPos = LPMTFile->Get<TTree>("PMTMap");
    LPMTPos->SetBranchAddress("x",&x);
    LPMTPos->SetBranchAddress("y",&y);
    LPMTPos->SetBranchAddress("z",&z);
    for(int pmtid = 0; pmtid<LPMTPos->GetEntries(); pmtid++){
        LPMTPos->GetEntry(pmtid);
        TVector3 temp_pos(x,y,z);
        lpmts.push_back(PMTinfo(pmtid, temp_pos));
        lpmts.back().ConvertToPixel();
    }
    LPMTFile->Close();


    // Read position of each SPMT
    /*TFile* SPMTFile = new TFile("/junofs/users/yuchincheng/NTU/ML/PMTMap/CD_SPMT.root");
    TTree* SPMTPos = SPMTFile->Get<TTree>("PMTMap");
    SPMTPos->SetBranchAddress("x",&x);
    SPMTPos->SetBranchAddress("y",&y);
    SPMTPos->SetBranchAddress("z",&z);
    for(int pmtid = 0; pmtid<SPMTPos->GetEntries(); pmtid++){
        SPMTPos->GetEntry(pmtid);
        TVector3 temp_pos(x,y,z);
        spmts.push_back(PMTinfo(pmtid, temp_pos));
        spmts.back().ConvertToPixel();
    }
    SPMTFile->Close();*/


    const string inpath = argv[1];
    const TString outpath = argv[2];
    const int pos1 = inpath.rfind("_");
    const int pos2 = inpath.rfind(".root");
    const int fileno = stoi(inpath.substr(pos1+1,pos2-pos1-1));   // Calculate the index of file number

    
    vector<double> *LPMT_FHT = nullptr, *LPMT_HitTime = nullptr;
    vector<double> *SPMT_FHT = nullptr, *SPMT_HitTime = nullptr;
    vector<int> *LPMT_ID = nullptr, *SPMT_ID = nullptr;
    TFile* infile = new TFile(inpath.c_str());
    TTree* intree = infile->Get<TTree>("TRec");
    intree->SetBranchAddress("LPMT_FHT", &LPMT_FHT);            // LPMT first hit time with simlation and reconstruction (already sorted by PMTID)
    intree->SetBranchAddress("LPMT_HitTime", &LPMT_HitTime);    // LPMT true hit time, some LPMTs may be hit more than once in a single phys evt
    intree->SetBranchAddress("LPMT_PMTID", &LPMT_ID);           // LPMT ID corresponds to elements in LPMT_HitTime vector (1 to 1 relationship)

    /*intree->SetBranchAddress("SPMT_FHT", &SPMT_FHT);            // SPMT first hit time with simlation and reconstruction (already sorted by PMTID)
    intree->SetBranchAddress("SPMT_HitTime", &SPMT_HitTime);    // SPMT true hit time, some SPMTs may be hit more than once in a single phys evt
    intree->SetBranchAddress("SPMT_PMTID", &SPMT_ID);           // SPMT ID corresponds to elements in SPMT_HitTime vector (1 to 1 relationship)*/


    const int maxevents = 1000;
    const int totalevents = intree->GetEntries();
    for(int event = 0; event<min(maxevents,totalevents); ++event){

        intree->GetEntry(event);
        
        for(int pmtid = 0; pmtid<LPMT_FHT->size(); ++pmtid){
            lpmts.at(pmtid).Initialize();
            lpmts.at(pmtid).SetFirstHitTime(LPMT_FHT->at(pmtid));
        }

        for(int nhit = 0; nhit<LPMT_ID->size(); ++nhit){
            int pmtid = LPMT_ID->at(nhit);
            lpmts.at(pmtid).CheckandSetTrueHitTime(LPMT_HitTime->at(nhit));
        }

        /*for(int pmtid = 0; pmtid<SPMT_FHT->size(); pmtid++){
            spmts.at(pmtid).SetFirstHitTime(SPMT_FHT->at(pmtid));
        }

        for(int nhit = 0; nhit<SPMT_ID->GetEntries(); nhit++){
            int pmtid = SPMT_ID->at(nhit);
            spmts.at(pmtid).CheckandSetTrueHitTime(SPMT_HitTime->at(nhit));
        }*/


        ofstream outfile_lowres(outpath + TString::Format("/low_res/event_%d.csv",maxevents*(fileno-1)+event));
        ofstream outfile_highres(outpath + TString::Format("/high_res/event_%d.csv",maxevents*(fileno-1)+event));
        for(auto lpmt : lpmts){
            lpmt.ShiftandNormalize(shift, normalization);
            outfile_lowres << lpmt.GetPMTID() << "\t" << lpmt.GetPixelX() << "\t" << lpmt.GetPixelY();
            outfile_lowres << "\t" << lpmt.GetFirstHitTime() << endl;
            
            outfile_highres << lpmt.GetPMTID() << "\t" << lpmt.GetPixelX() << "\t" << lpmt.GetPixelY();
            outfile_highres << "\t" << lpmt.GetTrueHitTime() << endl;
        }


        //for(auto spmt : spmts) spmt.ShiftandNormalize(shift, normalization);
        outfile_lowres.close();
        outfile_highres.close();

    }

    
}



