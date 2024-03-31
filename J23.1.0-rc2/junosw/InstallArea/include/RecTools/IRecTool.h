#ifndef IRECTOOL_H
#define IRECTOOL_H

#include "RecTools/PMTTable.h"

class IRecTool{
    public:
        IRecTool();
        virtual ~IRecTool();

        virtual bool configure() = 0;
        virtual void rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &) = 0;

    private:
};
#endif
