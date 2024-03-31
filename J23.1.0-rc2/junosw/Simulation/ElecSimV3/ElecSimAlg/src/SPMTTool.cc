#include "SPMTTool.h"

#include <iostream>

DECLARE_TOOL(SPMTTool);

Pulse SPMTTool::generate(int pmtid, TimeStamp hittime, double weight, bool iftruths) {
//  LogDebug <<  " generating pulse: pmt " << pmtid << std::endl;
  Pulse p;
  p.pmtID = pmtid;
  try{
    double PE2ADC_MEAN = m_SpmtConfigSvc->getDouble("PE2ADC_MEAN");
    double PE2ADC_STDD = m_SpmtConfigSvc->getDouble("PE2ADC_STDD");
    double TT_MEAN =  m_SpmtConfigSvc->getDouble("TT_MEAN");
    double TT_STDD =  m_SpmtConfigSvc->getDouble("TT_STDD");
    double amplitude = generator.Gaus(PE2ADC_MEAN*weight,PE2ADC_STDD);
    p.amplitude = ( (amplitude>0) ? amplitude : 0 ); 
    if(iftruths)
    {
	p.AddTruth();
	p.truth->TTS = generator.Gaus(TT_MEAN,TT_STDD)*1e-9;
	hittime.Add(p.truth->TTS);
    }
    else hittime.Add(generator.Gaus(TT_MEAN,TT_STDD)*1e-9);
    p.pulseHitTime = hittime;
    p.type = kNormalPulse;
  }
  catch(std::out_of_range e){
    LogError << e.what() << std::endl;
  }

  return p;
}

void SPMTTool::setSpmtConfig(SpmtElecConfigSvc* aSvc){
  m_SpmtConfigSvc=aSvc;
  try{
    generator.SetSeed(m_SpmtConfigSvc->getInt("SEEDRAND"));
  }
  catch(std::out_of_range e){
    LogError << e.what() << std::endl;
  }
}


