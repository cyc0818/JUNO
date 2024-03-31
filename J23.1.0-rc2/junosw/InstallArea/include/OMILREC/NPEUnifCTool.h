//need a review
#ifndef NPEUnifCTool_h
#define NPEUnifCTool_h

#include "IRecTool.h"
#include "RecTools/IRecTool.h"
#include "RecTools/PMTTable.h"
#include <memory>

class NPEUnifCTool: public ToolBase, public IRecTool {
public:
    NPEUnifCTool(const std::string& name);
    ~NPEUnifCTool();
    void rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo);
    bool configure();

private:
    const TH2F* NormNPEMap;
};

#endif
