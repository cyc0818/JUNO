/*
TopTracker Digitization

Author: Karaparambil Rebin, J. P. A. M. de Andre (IPHC)

Performs digitization of TopTracker signal
Based on the TT digitization written by A.Meregaglia, C.Jollet

*/



#ifndef TTDigitization_h
#define TTDigitization_h


#include <vector>

#include "Event/SimHeader.h"
#include "EvtNavigator/NavBuffer.h"
#include "Geometry/TTGeomSvc.h"
#include "ElecSimSvc.h"

#include<iostream>
using namespace std;

class TTDigitization {
 public:
  TTDigitization();
  ~TTDigitization();

  bool initialize(JM::SimEvt* evt, std::vector<std::vector<int>> & CTvec);

  bool computeDigitTT(std::vector<int> & d_Channel, std::vector<float> & d_Edep, std::vector<int> & d_Time, std::vector<std::vector<int>> & d_CTvec);

  void readFileCT(std::vector<std::vector<int>> & vec);

  int get_TotalChannels();

  const std::vector<int> &get_channelID() const;
  const std::vector<float> &get_npe() const;
  const std::vector<int> &get_adc() const;
  const std::vector<int> &get_time() const;


 private:

  //Used for global input output
  std::vector<int> channel;
  std::vector<float> npe;
  std::vector<int> adc;
  std::vector<int> time;  // ns
  int m_NChannels;

  int CTvalues[64][21];

 public:
  double get_TimeWalk(double val_pe);

};

#endif
