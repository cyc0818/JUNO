//to do review
#ifndef ZYQCTOOL_H
#define ZYQCTOOL_H
#include "RecTools/IRecTool.h"
#include "SniperKernel/ToolBase.h"

class zyQCTool: public ToolBase, public IRecTool{
    public:
        zyQCTool(const std::string &);
        virtual ~zyQCTool();

        bool configure();
        void rec(HitPMTVector &hitpmtvector, PMTTable &pmttable, RecInfo &recinfo);

        struct Hit{
            unsigned int pmtid; 
            float hittime;
            Hit(unsigned int pmtid=-1u, float hittime=0)
                : pmtid(pmtid), hittime(hittime) {
                }
            bool operator < (const Hit rhs) const {
                return this->hittime<rhs.hittime;
            }
        };

    private:
        std::vector<Hit> m_Hit_buffer;
};
#endif
