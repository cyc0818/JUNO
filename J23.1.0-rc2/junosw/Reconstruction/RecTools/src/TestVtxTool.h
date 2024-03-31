#ifndef TESTVTXTOOL_H
#define TESTVTXTOOL_H

#include "SniperKernel/ToolBase.h"
#include "RecTools/PMTTable.h"
#include "RecTools/IRecTool.h"

class TestVtxTool: public ToolBase, public IRecTool{
    public:
        TestVtxTool(const std::string &);
        virtual ~TestVtxTool();

        virtual bool configure();
        virtual void rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo);

    private:
};
#endif
