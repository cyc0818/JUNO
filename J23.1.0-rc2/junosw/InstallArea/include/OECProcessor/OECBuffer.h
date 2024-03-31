#ifndef OECBUFFER_H
#define OECBUFFER_H

#include "EvtNavigator/NavBuffer.h"

class OECBuffer: public JM::NavBuffer{
    public:
        OECBuffer(double, double);
        ~OECBuffer();

        ////return iterator for OECBuffer
        //typedef std::deque<std::shared_ptr<JM::EvtNavigator>>::reverse_iterator rIterator;
        //inline rIterator rbegin();
        //inline rIterator rcurrent();//the previous event of current event
        //inline rIterator rend();


        bool next();
        bool adopt(ElementPtr&);
        double t2Floor(int no=0);//no: position relative to current event
        double t2Ceiling(int no=0);

        std::shared_ptr<JM::EvtNavigator> realPop();//pop m_tmpBuf
        void popAll();//pop all events in m_tmpBuf

    private:
        bool updateFloor();

        std::deque<std::shared_ptr<JM::EvtNavigator>> m_tmpBuf;//store events already out of time window

        const double d_floor;//unit: s
        const double d_ceiling;
};
#endif
