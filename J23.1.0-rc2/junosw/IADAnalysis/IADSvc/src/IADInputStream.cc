#include "IADSvc/IADInputStream.h"
#include "IADFormat/IndexDict.h"
#include "IADFormat/MetaBlock.h"

IADInputStream:: IADInputStream(std::string inputFile): m_block(nullptr){
    m_reader = new IAD::IADFileReader(inputFile);

    auto metaBuf = m_reader->getMetaBuffer();
    IAD::MetaBlock meta(metaBuf);
    auto m = meta.getMap();
    IAD::IndexDict::instance()->insertMap(m);
    LogInfo<<"index address"<<m[0];
}

bool IADInputStream::next(){
    if(!m_block || !m_block->next()){
        if(!m_reader->next())   return false;

        m_block = std::make_unique<IAD::Block>(m_reader->getData());

        if(!m_block->next()){
            return next();
        }
    }
    return true;
}

IADEvtBox IADInputStream::get(){
    IAD::EventHeader* _h = (IAD::EventHeader*)m_block->getCur();
    if(_h->marker != IAD::Base_header_marker){
        throw std::runtime_error("Error: Failed to get evt. Wrong marker");
    }

    IADEvtBox res;
    res.evtHeader = _h;
    return res;
}