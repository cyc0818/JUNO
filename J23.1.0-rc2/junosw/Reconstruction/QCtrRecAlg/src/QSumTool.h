//to do review
#ifndef QSUMTOOL_H
#define QSUMTOOL_H
#include "RecTools/IRecTool.h"
#include "SniperKernel/ToolBase.h"
#include <vector>

class QSumTool: public ToolBase, public IRecTool{
    public:
        QSumTool(const std::string &);
        virtual ~QSumTool();

        bool configure();
        void rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo);

    private:
        double d_lightYield;
        double nHESF;

};
#endif
