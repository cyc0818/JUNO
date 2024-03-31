/*
TopTracker Digitization

Author: Karaparambil Rebin, J. P. A. M. de Andre (IPHC)

Performs digitization of TopTracker signal
Based on the TT digitization written by A.Meregaglia, C.Jollet

*/


#include "Randomize.hh"
#include "G4Poisson.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"


#include "TTDigitization.h"

#include "Event/GenHeader.h"
#include "Event/SimHeader.h"
#include "EvtNavigator/EvtNavHelper.h"


#include "Identifier/TtID.h"

#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>


TTDigitization::TTDigitization(){
}


TTDigitization::~TTDigitization(){
}


bool TTDigitization::initialize(JM::SimEvt* evt, std::vector<std::vector<int>> & CTvec){

  channel.clear();
  npe.clear();
  adc.clear();
  time.clear();

 
  const auto& tt_deposits = evt->getTTHitsVec();

  //Fill information at Detsim level: edepL, edepR, tdep, timeR, timeL
  std::vector<int> m_dep_ch;
  std::vector<int> m_dep_t;
  std::vector<float> m_dep_pe;

  m_dep_ch.clear();
  m_dep_t.clear();
  m_dep_pe.clear();

  for (auto it_dep = tt_deposits.begin(); it_dep != tt_deposits.end(); ++it_dep)
    {
      auto dep=*it_dep;

      m_dep_ch.push_back(dep->getChannelID());
      m_dep_pe.push_back(dep->getEdep());
      m_dep_t.push_back(round(dep->getTime()));  // ns

    } //finsh fill

  //Apply the digitization  
  computeDigitTT(m_dep_ch, m_dep_pe, m_dep_t, CTvec);

  return true;
}

bool TTDigitization::computeDigitTT(std::vector<int> & stp_Channel, std::vector<float> & stp_Edep, std::vector<int> & stp_Time, std::vector<std::vector<int>> & d_CTvec){

  std::vector<int> d_Channel;
  std::vector<float> d_Edep;
  std::vector<int> d_Time;
  std::vector<int> d_Adc;
  
  //bool file = false;
  // if(!file)
  //{
  //d_CTvec.clear();
  //readFileCT(d_CTvec);
  //}

  //Begin grouping
  int ndeposits = stp_Channel.size();
  int ntouch(0);
  int found(0);
  int ch(0);

  for(int i=0; i<ndeposits; i++)
    {
      ch = stp_Channel[i];
      found = 0;

      for(int kk=0; kk<ntouch; kk++)
        {
          if(ch==d_Channel[kk])
            {

              if(d_Edep[kk] < 1./3) d_Time[kk] = stp_Time[i];
              d_Edep[kk] += G4Poisson(stp_Edep[i]);

              found=1;
              break;

            }
        }

      if(found==0)
        {
          d_Channel.push_back(ch);
          d_Time.push_back(stp_Time[i]);
          d_Edep.push_back(G4Poisson(stp_Edep[i]));

          ntouch++;
        }

    }


  //Begin cross-talk

  int ntouchLoop = ntouch;

  int newchannel(0);
  int newchannelID(0);
  double probCT(0);


  for(int i=0; i< ntouchLoop; i++)
    {
      int hitChannelID = d_Channel[i];   //Channel ID of the present PE deposit before cross-talk
      auto IdTT = TtID::id(hitChannelID);
      int hitWallID = TtID::wall_id(IdTT);  //Get wall ID of present channel
      int hitPmtID = TtID::pmt(IdTT);       //Get PMT ID of present channel
      int hitStripID = TtID::strip(IdTT);   //Get Strip ID present channel (before cross-talk)
      int peLoop = d_Edep[i];

      for (int k=0; k<peLoop; k++)         //Cross-talk performed for each single PE per channel
        {
          //For each PE, cross-talk probability is calculated
          //If there is cross-talk, the PE is substracted from initial channel and allocated to new channel (Time of PE is kept same)
          probCT=G4UniformRand();
          if(probCT<=0.92496){newchannel=0;}
          if(probCT>0.92496 && probCT<=0.93806){newchannel=d_CTvec[hitStripID][1];}
          if(probCT>0.93806 &&probCT<=0.95116){newchannel=d_CTvec[hitStripID][2];}
          if(probCT>0.95116 && probCT<=0.96426){newchannel=d_CTvec[hitStripID][3];}
          if(probCT>0.96426 && probCT<=0.97736){newchannel=d_CTvec[hitStripID][4];}
          if(probCT>0.97736 && probCT<=0.98046){newchannel=d_CTvec[hitStripID][5];}
          if(probCT>0.98046 && probCT<=0.98356){newchannel=d_CTvec[hitStripID][6];}
          if(probCT>0.98356 && probCT<=0.98666){newchannel=d_CTvec[hitStripID][7];}
          if(probCT>0.98666 && probCT<=0.98976){newchannel=d_CTvec[hitStripID][8];}
          if(probCT>0.98976 && probCT<=0.99106){newchannel=d_CTvec[hitStripID][9];}
          if(probCT>0.99106 && probCT<=0.99236){newchannel=d_CTvec[hitStripID][10];}
          if(probCT>0.99236 && probCT<=0.99366){newchannel=d_CTvec[hitStripID][11];}
          if(probCT>0.99366 && probCT<=0.99496){newchannel=d_CTvec[hitStripID][12];}
          if(probCT>0.99496 && probCT<=0.99559){newchannel=d_CTvec[hitStripID][13];}
          if(probCT>0.99559 && probCT<=0.99622){newchannel=d_CTvec[hitStripID][14];}
          if(probCT>0.99622 && probCT<=0.99685){newchannel=d_CTvec[hitStripID][15];}
          if(probCT>0.99685 && probCT<=0.99748){newchannel=d_CTvec[hitStripID][16];}
          if(probCT>0.99748 && probCT<=0.99811){newchannel=d_CTvec[hitStripID][17];}
          if(probCT>0.99811 && probCT<=0.99874){newchannel=d_CTvec[hitStripID][18];}
          if(probCT>0.99874 && probCT<=0.99937){newchannel=d_CTvec[hitStripID][19];}
          if(probCT>0.99937 && probCT<=1){newchannel=d_CTvec[hitStripID][20];}


          if(newchannel!=0&&newchannel!=99)
            {

              d_Edep[i]-=1;

              newchannelID = TtID::getIntID(hitWallID, hitPmtID, newchannel);

              found=0;
              for(int kk=0; kk<ntouch; kk++)
                {

                  if(newchannelID==d_Channel[kk])
                    {
                      d_Edep[kk]+=1;

                      found=1;
                      break;

                    }

                }


              if(found==0)
                {
                  d_Channel.push_back(newchannelID);
                  d_Time.push_back(d_Time[i]);
                  d_Edep.push_back(1);
                  ntouch++;
                }

            }



        }//End of assigning each PE per CT channel


    }//End of channel loop: end of cross-talk


  //Begin of PE to ADC conversion: mean gain 40 AC units and 50% resolution

  double meanGain(40); //mean gain in ADC units
  int ntouchChannel(0);

  for (int i=0; i<ntouch; i++)
    {

      //Apply charge resolution of 50%
      d_Edep[i] = G4RandGauss::shoot(d_Edep[i], sqrt(d_Edep[i])*0.5);

      //Implement Time walk
      double timeWalk = get_TimeWalk(d_Edep[i]);
      d_Time[i] = d_Time[i] + timeWalk;

      //ADC conversion
      d_Adc.push_back(round(d_Edep[i]*meanGain));

      if(d_Adc[i]<0)
        d_Adc[i] = 0;

      if(d_Adc[i]>4096)
        d_Adc[i] = 4096;

      d_Edep[i] = d_Adc[i]/meanGain;

      if(d_Edep[i] > 0.34) //drop events not triggered (1/3 p.e.)  ????
        {
          //write digitization output
          channel.push_back(d_Channel[i]);
          npe.push_back(d_Edep[i]);
          adc.push_back(d_Adc[i]);
          time.push_back(d_Time[i]);
          ntouchChannel++;
                                }

    }

  m_NChannels = ntouchChannel;

  d_Channel.clear();
  d_Time.clear();
  d_Edep.clear();
  d_Adc.clear();

  return true;

}

void TTDigitization::readFileCT(std::vector<std::vector<int>> & vec){

  std::string path = getenv("JUNOTOP");
  path += "/data/Detector/Geometry/TopTracker/CrossTalk.txt";

  std::ifstream input(path.c_str());
  std::string line;

  int chnl;
  int neighbour;
  int position;

  while (input.good()){
    std::getline(input, line);
    if (input.fail()) { break; }

    std::stringstream stream;
    stream << line;

    stream >> chnl;
    if (stream.fail()) {continue;}

    stream >> position;
    if (stream.fail()) {continue;}

    stream >> neighbour;
    if (stream.fail()) {continue;}

    CTvalues[chnl][position]=neighbour;

  }

  //Fill CTvector
  for(int ii=0; ii<64; ii++){
    std::vector<int> row;
    for(int kk=0; kk<21; kk++){
      row.push_back(CTvalues[ii][kk]);
    }
    vec.push_back(row);
  }

}

const std::vector<int> & TTDigitization::get_channelID() const{

  return channel;
}

const std::vector<float> & TTDigitization::get_npe() const{

  return npe;
}

const std::vector<int> & TTDigitization::get_adc() const{

  return adc;
}

const std::vector<int> & TTDigitization::get_time() const{

  return time;
}

int TTDigitization::get_TotalChannels(){

  return m_NChannels;
}

double TTDigitization::get_TimeWalk(double val_PE){

  if(val_PE <= 0){
    return 0;
  }

  //Begin calibration of time walk
  //k = log10( PE ); T(k) = P_0 + [ (P_1 * EXP{-kP_2}) + (P_3 * EXP{-kP_4}) ] 

  double P_0 = 35.5499;
  double P_1 = 7.73813e-5;
  double P_2 = 21.8316;
  double P_3 = 5.96284;
  double P_4 = 1.27443;

  double k = log10(val_PE);
  double beta1 = -k*P_2;
  double beta2 = -k*P_4;

  double tw = P_0 + ((P_1*exp(beta1)) + (P_3*exp(beta2)));

  return tw;
}
