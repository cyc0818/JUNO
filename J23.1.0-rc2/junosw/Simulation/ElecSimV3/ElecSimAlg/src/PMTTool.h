#ifndef PMTTOOL_H
#define PMTTOOL_H

#include "IPMTTool.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"

class PMTTool: public ToolBase, public IPMTTool {
public:
    PMTTool(const std::string& name): ToolBase(name) {

    }

    ~PMTTool() {

    }

    Pulse generate(int pmtid, TimeStamp hittime, double weight, bool iftruths);

};


#endif //PMTTOOL
