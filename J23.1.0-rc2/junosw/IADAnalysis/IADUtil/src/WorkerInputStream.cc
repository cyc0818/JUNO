#include "IADUtil/WorkerInputStream.h"
#include "EvtNavigator/EvtNavigator.h"
#include "IADFormat/IADDefine.h"
#include "SniperKernel/SniperLog.h"
#include "TROOT.h"

WorkerInputStream::WorkerInputStream() : m_sname("IADGlobalStream"), m_block(nullptr), m_elem(nullptr)
{
    LogInfo << "Create Worker inputstream" << std::endl;
}

WorkerInputStream::WorkerInputStream(std::vector<std::string> fileList) {}

bool WorkerInputStream::initialize()
{
    m_gbuf = IADGlobalStream::GetBufferFrom(m_sname);
    LogInfo << "worker inputstream init" << std::endl;
    return true;
}

bool WorkerInputStream::finalize()
{

    return true;
}

bool WorkerInputStream::first(bool read)
{
    return next();
}

bool WorkerInputStream::next(int step, bool read)
{
    if (!m_block || !m_block->next())
    { // run out of current block
        m_elem = m_gbuf->next();
        if (m_elem == nullptr)
        {
            return false;
        }
        // LogInfo<<"Worker catch one block"<<std::endl;
        m_block = std::make_unique<IAD::Block>(m_elem->dptr->buffer);

        // Fixme: Only after the whole block have been processed, should it be set done
        if (m_elem != nullptr)
        {
            m_gbuf->setDone(m_elem);
        }

        if (!m_block->next())
        {
            return next();
        }
    }
    return true;
}

IADEvtBox WorkerInputStream::get()
{
    // std::cout<<"Try to get a nav "<<std::endl;
    IAD::EventHeader *_h = (IAD::EventHeader *)m_block->getCur();
    if(_h->marker != IAD::Base_header_marker){
        throw std::runtime_error("Error: Failed to get evt. Wrong marker");
    }
    IADEvtBox res;
    res.evtHeader = _h;

    return res;
}
