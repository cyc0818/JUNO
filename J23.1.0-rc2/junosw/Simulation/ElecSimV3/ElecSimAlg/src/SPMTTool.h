#ifndef SPMTTOOL_H
#define SPMTTOOL_H

#include "IPMTTool.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "TRandom3.h"

#include "SpmtElecConfigSvc/SpmtElecConfigSvc.h"

class SPMTTool: public ToolBase, public IPMTTool {
  public:
    SPMTTool(const std::string& name): ToolBase(name), generator(0) {

    }

    ~SPMTTool() {

    }

    void setSpmtConfig(SpmtElecConfigSvc* aSvc);

    Pulse generate(int pmtid, TimeStamp hittime, double weight, bool iftruths);

  private:

    SpmtElecConfigSvc* m_SpmtConfigSvc;

    TRandom3 generator;

};

#endif //SPMTTOOL_H
