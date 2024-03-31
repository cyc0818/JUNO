
#include "GenTools/GenEventBuffer.h"

GenEventBuffer* GenEventBuffer::m_instance=NULL;

GenEventBuffer*
GenEventBuffer::instance()
{
    if (!m_instance) {
        m_instance = new GenEventBuffer;
    }

    return m_instance;
}

bool
GenEventBuffer::push_back(GenEventPtr ge)
{
    m_buffer.push(ge);
    return true;
}

bool
GenEventBuffer::pop_front(GenEventPtr &ge)
{
    if (m_buffer.empty()) {
        return false;
    }
    ge = m_buffer.front();
    m_buffer.pop();
    return true;
}

bool
GenEventBuffer::front(GenEventPtr &ge)
{
    if (m_buffer.empty()) {
        return false;
    }
    ge = m_buffer.front();
    return true;
}
