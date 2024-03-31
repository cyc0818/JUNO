#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TGraph.h"
#include <TROOT.h>

using namespace std;
class MyTree{

  public:
    ;


};


class FadcTree {
  public:
    FadcTree(const char* outputname,int nChn =16720,bool isMC = true) 
    : m_outFile(outputname,"recreate"), m_outTree("fadcTree","fadcTree") 
    {
      nChn_ = nChn;
      Reset();
      m_outTree.Branch("nChn", &nChn_, "nChn/I");
      //m_outTree.Branch("recE", &recE, "recE/F");
      //m_outTree.Branch("recX", &recX, "recX/F");
      //m_outTree.Branch("recY", &recY, "recY/F");
      //m_outTree.Branch("recZ", &recZ, "recZ/F");
      //m_outTree.Branch("chId", &chId, "chId[nChn]/I");
      //m_outTree.Branch("ring", &ring, "ring[nChn]/I");
      //m_outTree.Branch("clmn", &clmn, "clmn[nChn]/I");
      //m_outTree.Branch("feeQ", &feeQ, "feeQ[nChn]/F");
      m_outTree.Branch("fdcQ", &fdcQ, "fdcQ[nChn]/F");
      m_outTree.Branch("faPT", &faPT, "faPT[nChn]/F");
    //  m_outTree.Branch("fdcP", &fdcP, "fdcP[nChn]/F");
      m_outTree.Branch("chi2", &chi2, "chi2[nChn]/F");
   //   m_outTree.Branch("isSt", &isSt, "isSt[nChn]/I");
    //  m_outTree.Branch("fxOs", &fxOs, "fxOs[nChn]/I");
    //  m_outTree.Branch("amOs", &amOs, "amOs[nChn]/F");
    //  m_outTree.Branch("faPN", &faPN, "faPN[nChn]/I");
    //  m_outTree.Branch("faPT", &faPT, "faPT[nChn][50]/F");
    //  m_outTree.Branch("faPQ", &faPQ, "faPQ[nChn][50]/F");
      //m_outTree.Branch("zeyQ", &zeyQ, "zeyQ[nChn]/F");
      //m_outTree.Branch("zeyN", &zeyN, "zeyN[nChn]/I");
      
      if(isMC){
   //     m_outTree.Branch("truE", &truE, "truE/F");
   //     m_outTree.Branch("nHit", &nHit, "nHit[nChn]/I");
   //     m_outTree.Branch("truHT", &truHT, "truHT[nChn]/F");
       // m_outTree.Branch("truQ", &truQ, "truQ[nChn]/F");
       // m_outTree.Branch("truN", &truN, "truN[nChn]/I");
        //m_outTree.Branch("truP", &truP, "truP[nChn]/I");
        //m_outTree.Branch("nPul", &nPul, "nPul[nChn]/I");
        //m_outTree.Branch("trPT", &trPT, "trPT[nChn][50]/F");
      }
    }
    void Reset() {
     // recE = -1;
    //  recX = 0;
    //  recY = 0;
    //  recZ = 0;
      truE = -1;

    //  truX = 0;
    //  truY = 0;
    //  truZ = 0;
      for(int i=0; i<s_maxChn; i++) {
      nHit[i] = -1;  
      fdcQ[i] = -1;
      faPT[i] = -1;
      //truHT[i] = -1;
      chi2[i] = -1;
      // chId[i] = -1;
       // ring[i] = -1;
       // clmn[i] = -1;
       // feeQ[i] = -1;
       // fdcQ[i] = -1;
       // fdcP[i] = -1;
       // faPN[i] = -1;
       // chi2[i] = -1;
       // isSt[i] = -1;
       // fxOs[i] = -1;
       // amOs[i] = -1;
       // zeyQ[i] = -1;
       // zeyN[i] = -1;
       // 
       // truQ[i] = -1;
       // truN[i] = -1;
       // truP[i] = -1;
       // nPul[i] = -1;
       // nHit[i] = -1;
       // for(int j=0; j<50; j++)  {
       //   trHT[i][j] = -1;
       //   trPT[i][j] = -1;
       //   faPT[i][j] = -1;
       //   faPQ[i][j] = -1;
       // }
      
      
      }
    }
    void Fill() {m_outTree.Fill();}
    void Write(){
      m_outFile.cd();
      m_outTree.Write();
      m_outFile.Close();
    }
    static const int s_maxChn = 16720;
    // float recE;
    // float recX;
    // float recY;
    // float recZ;
    // int   chId[s_maxChn];
    // int   ring[s_maxChn];
    // int   clmn[s_maxChn];
    // float feeQ[s_maxChn];
     float fdcQ[s_maxChn];
    // float fdcP[s_maxChn];
     float chi2[s_maxChn];
    // int   isSt[s_maxChn];
    // int   fxOs[s_maxChn];
    // float amOs[s_maxChn];
    // int   faPN[s_maxChn];
    // float faPT[s_maxChn][50];
     float faPT[s_maxChn];
    // float faPQ[s_maxChn][50];
    // float zeyQ[s_maxChn];
    // int   zeyN[s_maxChn];
    // 
     float truE;
    // float truX;
    // float truY;
    // float truZ;
    // float truQ[s_maxChn];
    // int   truN[s_maxChn];
    // int   truP[s_maxChn];
    // int   nPul[s_maxChn];
     int   nHit[s_maxChn];
     float trHT[s_maxChn];
    // float trPT[s_maxChn][50];
    // float trHT[s_maxChn][50];
  private:
    //static const int s_maxHit = 5000;
    int nChn_;
    TFile m_outFile;
    TTree m_outTree;
};
