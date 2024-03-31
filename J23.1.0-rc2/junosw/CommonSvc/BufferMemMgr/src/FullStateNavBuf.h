#ifndef FULL_STATE_NAV_BUF_H
#define FULL_STATE_NAV_BUF_H

#include "EvtNavigator/NavBuffer.h"
#include <memory>

class IInputStream;

class FullStateNavBuf : public JM::NavBuffer
{
    public :

        FullStateNavBuf(double lowBound, double highBound);

        virtual ~FullStateNavBuf();

        void init(IInputStream* iStream);

        bool next();

        bool adopt(std::shared_ptr<JM::EvtNavigator> &nav);

        bool reset(int entry);

    private :

        void trimDated();
        void fillFresh();
        void fillNext();
        double timeShift(JM::EvtNavigator* nav);

        int                m_rEntry;
        const double       m_floor;
        const double       m_ceiling;

        JM::EvtNavigator*  m_beyond;
        IInputStream*      m_iStream;
};

#endif
