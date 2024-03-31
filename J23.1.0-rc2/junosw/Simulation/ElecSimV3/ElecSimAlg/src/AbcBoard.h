#ifndef AbcBoard_h
#define AbcBoard_h

#include <vector>
#include <map>
#include "TF1.h"

#include "Event/CdSpmtElecHeader.h"
#include "Event/CdSpmtElecTruthHeader.h"

#include "Pulse.h"
#include "SpmtElecConfigSvc/SpmtElecConfigSvc.h"


class AbcBoard {
  public:
    AbcBoard(const int num, SpmtElecConfigSvc* spmtSvc);
    ~AbcBoard();

    void initialize();
    void delFunc();
    void reset(TimeStamp time){initTime=time;}

    void pushPulses(int channel, std::vector<Pulse*> pulses);
    void process( JM::CdSpmtElecEvt* spmtEvent, JM::CdSpmtElecTruthEvt* spmtElecTruthEvent);

    void clearPulses(){pulsesMap.clear();}
    void AddTruth(JM::CdSpmtElecTruthEvt* spmtElecTruthEvent, std::vector<Pulse*>::iterator aPulse);    
    double Apply_ChargeAcceptance(int RCval, double t);
    bool trigger_deadtime_func(double t);
    void Apply_Catiroc(int RC, std::vector<double> ti,  std::vector<double> qi,  std::vector<double> &trig_qi, std::vector<double> &trig_ti, std::vector<double> &dds_ti);
    double GetRandomCharge(int qi_tmp);
    double GetTime(double random_amplitude, double t0, double threshold);

  private:

    int boardNum;
    TimeStamp initTime;

    SpmtElecConfigSvc* m_SpmtConfigSvc;

    std::map<int , std::vector<Pulse*> > pulsesMap;

    TF1* f2;
    TF1* func1;
    TF1* gauss_func;
    TF1* model_func;

};

#endif
