#include "PMTTool.h"
#include "IPMTTool.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"

DECLARE_TOOL(PMTTool);


Pulse PMTTool::generate(int pmtid, TimeStamp hittime, double weight, bool iftruths) {
  Pulse p;
  p.pmtID = pmtid;
  p.amplitude = weight;
  if(iftruths)
  {
	p.AddTruth();
	p.truth->TTS = 0.0;
  }
  p.pulseHitTime = hittime;
  p.type = kNormalPulse;
   
  return p;
}
