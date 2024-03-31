#include "KRLGeoFlux.hh"

KRLGeoFlux* gGeoFlux;

KRLGeoFlux::KRLGeoFlux(const std::string& fluxfile,
                       const std::string& reactorfile) {
  // read world reactor information (without Yangjiang and Taishan)
    ReactorFile = TFile::Open(reactorfile.c_str());
    hisReac = (TH1F*)ReactorFile->Get("hisreactor");

  // this is only for normalization of U and Th's flux
   fphiU=7.0*TMath::Power(10,6);    
   fphiTh=10.0*TMath::Power(10,6);
   // read U and Th spectrum from data
   std::ifstream fp1(fluxfile.c_str());
    hisU = new TH1F("hisU"," ",5000,0,5.0);
    hisTh = new TH1F("hisTh"," ",5000,0,5.0);
    hisUTh = new TH1F("hisUTh"," ",5000,0,5.0);
    Inline =0;
    while(getline(fp1, line))
     {

       std::stringstream ss(line);
       ss >> fEnergy >> fNe ;  

        // for Th specturm  
       if(Inline>4386&&Inline<6641){
        
        fphiETh=fNe*fphiTh;
        xaxisTh = hisTh->GetXaxis();
        binxTh = xaxisTh->FindBin(fEnergy/1000);
        //if(fEnergy/1000>1.806&&fEnergy/1000<2.25)
        hisTh->SetBinContent(binxTh,fphiETh);                                
        //else hisTh->SetBinContent(binxTh,0);
        }
        // for U spectrum
        if(Inline<4386){
        fphiEU=fNe*fphiU;
        xaxisU = hisU->GetXaxis();
        binxU = xaxisU->FindBin(fEnergy/1000);
        //if(fEnergy/1000>1.806&&fEnergy/1000<3.40)
        hisU->SetBinContent(binxU,fphiEU);                                
        //else  hisU->SetBinContent(binxU,0);
        }       
       
      Inline++;
      }
   hisUTh->Add(hisU);
   hisUTh->Add(hisTh);
//   hisU->Draw();

}
 void KRLGeoFlux::SetiGeocase(Int_t val)
{ 
  iGeo = val;
  printf("iGeoUTh is manually set %d \n", iGeo);
}


Double_t KRLGeoFlux::FluxGeo(Double_t aEnu) {
   Double_t answer;  
   if(iGeo==0){
     xaxisU = hisU->GetXaxis();
     answer = hisU->GetBinContent(xaxisU->FindBin(aEnu));
   } 

   if(iGeo==1){
     xaxisTh = hisTh->GetXaxis();
     answer = hisTh->GetBinContent(xaxisTh->FindBin(aEnu));
   } 

   if(iGeo==2){
     xaxisUTh = hisUTh->GetXaxis();
     answer = hisUTh->GetBinContent(xaxisUTh->FindBin(aEnu));
    }

   if(iGeo==3){
     xaxisWR = hisReac->GetXaxis();
     answer = hisReac->GetBinContent(xaxisWR->FindBin(aEnu));
   } 

  
   return answer;
}

