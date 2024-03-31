#include "IADUtil/IADBlockContext.h"
#include "SniperKernel/AlgBase.h"
#include "IADFormat/IADFileReader.h"
#include "IADFormat/IndexDict.h"
#include "IADFormat/MetaBlock.h"

class InputThreadAlg : public AlgBase
{
public:
    InputThreadAlg(const std::string &name);
    virtual ~InputThreadAlg();

    virtual bool initialize() override;
    virtual bool execute() override;
    virtual bool finalize() override;

private:
    IADGlobalBuffer *m_gbuf;
    std::string m_sname;
    std::vector<std::string> m_readAhead;
    std::string m_inputFile;
    IAD::IADFileReader* m_reader;
    
};

#include "SniperKernel/AlgFactory.h"
DECLARE_ALGORITHM(InputThreadAlg);

InputThreadAlg::InputThreadAlg(const std::string &name)
    : AlgBase(name),
      m_gbuf(nullptr)
{
    declProp("Stream", m_sname = "IADGlobalStream");
    declProp("IADFile", m_inputFile);
}

InputThreadAlg::~InputThreadAlg()
{
}

bool InputThreadAlg::initialize()
{   
    if(m_inputFile.size() == 0){
        return false;
    }
    m_gbuf = IADGlobalStream::GetBufferFrom(m_sname);

    m_reader = new IAD::IADFileReader(m_inputFile);
    auto metaBuf = m_reader->getMetaBuffer();
    IAD::MetaBlock meta(metaBuf);
    auto m = meta.getMap();
    IAD::IndexDict::instance()->insertMap(m);

    return true;
}

bool InputThreadAlg::execute()
{   
    if(!m_reader->next()){
        m_gbuf->push_back(std::shared_ptr<IADBlockContext>(nullptr));
        m_par->stop();
        return true;
    }

    m_gbuf->push_back(std::shared_ptr<IADBlockContext>(new IADBlockContext(m_reader->getData())));
    //m_gbuf->push_back(std::shared_ptr<IADBlockContext>(new IADBlockContext(nav)));

    return true;
}

bool InputThreadAlg::finalize()
{
    if (m_gbuf->status())
    {
        m_gbuf->push_back(std::shared_ptr<IADBlockContext>(nullptr));
    }
    return true;
}