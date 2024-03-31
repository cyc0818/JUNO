#ifndef GenEventBuffer_h
#define GenEventBuffer_h

#include "HepMC/GenEvent.h"
#include <queue>
#include <boost/shared_ptr.hpp>

// typedef boost::shared_ptr<HepMC::GenEvent> GenEventPtr;
typedef HepMC::GenEvent* GenEventPtr;

class GenEventBuffer {
    public:
        static GenEventBuffer* instance();

    public:
        bool push_back(GenEventPtr ge);
        bool pop_front(GenEventPtr &ge);
        bool front(GenEventPtr &ge);

    private:
        static GenEventBuffer* m_instance;

    private:
        // buffer using queue
        std::queue<GenEventPtr> m_buffer;
};

#endif
