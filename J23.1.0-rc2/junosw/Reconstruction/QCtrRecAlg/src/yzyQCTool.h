//to do review
#ifndef ZYQCTOOL_H
#define ZYQCTOOL_H
#include "RecTools/IRecTool.h"
#include "SniperKernel/ToolBase.h"

#include "TF1.h"

class yzyQCTool: public ToolBase, public IRecTool{
    public:
        yzyQCTool(const std::string &);
        virtual ~yzyQCTool();

        bool configure();
        void rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo);

    private:
        //options
        bool b_usePerfectVtx;

        // Correction functions
        TF1* m_efun = nullptr;            // energy correction function
        TF1* m_non_li_positron = nullptr; // non-linear correction for positron

        void recVtx(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo);
        void recE(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo);
};
#endif
