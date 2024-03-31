#include "EvtNavigator/EvtNavigator.h"
#include "EDMUtil/EDMManager.h"
#include <algorithm>
// #include <iostream>
ClassImp(JM::EvtNavigator)

// drop bits outside the range [R, L]
uint32_t get_bit_range(uint32_t b, size_t R, size_t L)
{   
    b >>= R;            // drop R rightmost bits
    b <<= (31 - L + R);  // drop L leftmost bits
    b >>= (31 - L + R);  // shift to right most
    return b;
}

// set bits in the range [R, L]
void set_bit_range(uint32_t& tar, uint32_t val, size_t R, size_t L)
{   
    val <<= R;
    for (size_t i=R; i<=L; ++i) {
        if (TESTBIT(val,i))
            SETBIT(tar,i);
        else
            CLRBIT(tar,i);
    }
}

JM::EvtNavigator::EvtNavigator()
    : m_EventID(0)
    , m_RunID(0)
    , m_eventType(0)
{
    this->Class()->IgnoreTObjectStreamer();
}

JM::EvtNavigator::~EvtNavigator()
{
    std::vector<JM::SmartRef*>::iterator it, end = m_refs.end();
    for (it = m_refs.begin(); it != end; ++it) {
        delete *it;
    }
}

JM::EvtNavigator::EvtNavigator(const JM::EvtNavigator& nav)
    : TObject(nav)
{
    this->init(nav);
}

JM::EvtNavigator& JM::EvtNavigator::operator=(const JM::EvtNavigator& nav)
{
    if (this != &nav) {
        TObject::operator=(nav);
        this->init(nav);
    }
    return *this;
}

void JM::EvtNavigator::init(const JM::EvtNavigator& nav)
{
    m_paths = nav.m_paths;
    m_writeFlag = nav.m_writeFlag;
    m_TimeStamp = nav.m_TimeStamp;
    m_EventID = nav.m_EventID;
    m_RunID = nav.m_RunID;
    m_eventType = nav.m_eventType;
    // Clear previous SmartRefs
    std::vector<SmartRef*>::iterator it, end = m_refs.end();
    for (it = m_refs.begin(); it != end; ++it) {
        delete *it;
    }
    // Copy new SmartRefs
    std::vector<SmartRef*>::const_iterator it2, end2 = nav.m_refs.end();
    for (it2 = nav.m_refs.begin(); it2 != end2; ++it2) {
        SmartRef* ref = new SmartRef(**it2);
        m_refs.push_back(ref);
    }
}

JM::HeaderObject* JM::EvtNavigator::getHeader(const std::string& path)
{
    JM::SmartRef* ref = this->getSmartRef(path);
    if (!ref) {
        return 0;
    }
    return static_cast<JM::HeaderObject*>(ref->GetObject());
}

std::vector<JM::HeaderObject*>
JM::EvtNavigator::getHeaders(const std::string& path)
{
    std::vector<JM::HeaderObject*> result;

    for (size_t i = 0; i < m_paths.size(); ++i) {
        const std::string& p = m_paths[i];
        if (p == path) {
            // get the smartref:
            JM::SmartRef* ref = m_refs[i];
            JM::HeaderObject* hdr = dynamic_cast<JM::HeaderObject*>(ref->GetObject());

            result.push_back(hdr);
        }
    }

    return result;
}

void JM::EvtNavigator::setHeaderEntry(const std::string& path, int entry)
{
    JM::SmartRef* ref = this->getSmartRef(path);
    if (!ref) {
        return;
    }
    ref->setEntry(entry);
}

void JM::EvtNavigator::addHeader(const std::string& path, JM::HeaderObject* header)
{
    m_paths.push_back(path);
    SmartRef* ref = new SmartRef();
    m_refs.push_back(ref);
    m_refs.back()->SetObject(header);
    m_writeFlag.push_back(true);
}

void JM::EvtNavigator::addHeader(JM::HeaderObject* header) 
{
    m_paths.push_back(EDMManager::get()->getPathWithHeader(header->ClassName()));
    SmartRef* ref = new SmartRef();
    m_refs.push_back(ref);
    m_refs.back()->SetObject(header);
    m_writeFlag.push_back(true);
}

bool JM::EvtNavigator::copyHeader(JM::EvtNavigator* another, const std::string& oldPath, const std::string& newPath)
{
    // Get old Header information
    JM::SmartRef* oref = another->getSmartRef(oldPath);
    if (0 == oref) return false;
    JM::HeaderObject* header = another->getHeader(oldPath);
    if (0 == header) return false;
    std::string path = newPath.size() ? newPath : oldPath;
    // Add Header
    m_paths.push_back(path);
    SmartRef* ref = new SmartRef();
    ref->setEntry(oref->entry());
    m_refs.push_back(ref);
    m_refs.back()->SetObject(header);
    m_writeFlag.push_back(true);
    return true;
}

void JM::EvtNavigator::adjustPath(const std::vector<std::string>& path)
{
    // // DEBUG BY TAO
    // std::cout << "JM::EvtNavigator::adjustPath BEGIN DEBUG. " << std::endl;
    // std::cout << "param path:" << path.size() << std::endl;
    // for (size_t i = 0; i < path.size(); ++i ) {
    //     std::cout << "path[" << i << "]: " << path[i] << std::endl;
    // }

    // std::cout << "m_paths: " << m_paths.size() << std::endl;
    // for (size_t i = 0; i < m_paths.size(); ++i ) {
    //     std::cout << "m_paths[" << i << "]: " << m_paths[i] << std::endl;
    // }
    
    // std::cout << "JM::EvtNavigator::adjustPath END DEBUG. " << std::endl;

    for (size_t i = 0; i < path.size(); ++i ) {
        std::vector<std::string>::iterator pos = std::find(m_paths.begin(), m_paths.end(),path[i]);
        if (pos == m_paths.end()) {
            // One new path, insert it just for occupying position
            m_paths.insert(m_paths.begin() + i, path[i]);
            m_refs.insert(m_refs.begin() + i, new SmartRef);
            m_writeFlag.insert(m_writeFlag.begin() + i, false);
        }
        else {
            size_t ps = pos - m_paths.begin();
            if (ps != i) {
                // We find the path, but it's on the wrong position
                std::swap(*pos, m_paths[i]);
                std::swap(m_refs[pos - m_paths.begin()], m_refs[i]);
                bool temp = m_writeFlag[pos - m_paths.begin()];
                m_writeFlag[pos - m_paths.begin()] = m_writeFlag[i];
                m_writeFlag[i] = temp;
            }
        }
    }
}

bool JM::EvtNavigator::writePath(const std::string& path)
{
    std::vector<std::string>::iterator pos = find(m_paths.begin(), m_paths.end(), path);
    if (m_paths.end() == pos) {
        return false;
    }
    if (! this->getSmartRef(path)) {
        return false;
    }
    if (! this->getSmartRef(path)->HasObject()) {
        return false;
    }
    return m_writeFlag[pos - m_paths.begin()];
}

void JM::EvtNavigator::setWriteFlag(const std::string& path, bool write)
{
    std::vector<std::string>::iterator pos = find(m_paths.begin(), m_paths.end(), path);
    if (m_paths.end() == pos) return;
    m_writeFlag[pos - m_paths.begin()] = write;
}

void JM::EvtNavigator::resetWriteFlag()
{
    if (m_writeFlag.size()) m_writeFlag.clear();
    int npath = m_paths.size();
    for (int i = 0; i < npath; ++i) {
        m_writeFlag.push_back(true);
    }
}

JM::SmartRef* JM::EvtNavigator::getSmartRef(const std::string& path)
{
    // std::cout << "DEBUG JM::EvtNavigator::getSmartRef("
    //           << path << ")" << std::endl;
    std::vector<std::string>::iterator pos = std::find(m_paths.begin(), m_paths.end(), path);
    if (m_paths.end() == pos) {
        // Users are responsible to check the pointer and give the warning. 
        // std::cout << "WARNING: EvtNavigator::getSmartRef can't find " << path << std::endl;
        return 0;
    }
    size_t ps = pos - m_paths.begin();
    // std::cout << "ps = pos - m_paths.begin(): " << ps << std::endl;
    // std::cout << "m_refs.size(): " << m_refs.size() << std::endl;
    // std::cout << "m_paths.size(): " << m_paths.size() << std::endl;

    if (ps >= m_refs.size()) {
        return 0;
    } 
    return m_refs[ps];
}
