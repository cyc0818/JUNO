#include "RawDataInputSvc.h"
#include "RawDataInputStream.h"
#include "SniperKernel/SvcFactory.h"

DECLARE_SERVICE(RawDataInputSvc);

RawDataInputSvc::RawDataInputSvc(const std::string& name)
    : SvcBase(name),
      m_inputStream(nullptr)
{
    declProp("Type",       m_type = "DAQ");
    declProp("InputFile",  m_files);
}

RawDataInputSvc::~RawDataInputSvc()
{
}

bool RawDataInputSvc::initialize()
{
    if ( m_files.empty() ) {
        LogError << "no input file is set" << std::endl;
        return false;
    }

    m_inputStream = new RawDataInputStream(m_files, m_type);

    bool stat = m_inputStream->initialize();
    if ( ! stat ) {
        LogError << "failed to initialize the input stream" << std::endl;
    }

    return stat;
}

bool RawDataInputSvc::finalize()
{
    bool stat = m_inputStream->finalize();

    delete m_inputStream;

    return stat;
}

IInputStream* RawDataInputSvc::getInputStream(const std::string& /*path*/)
{
    return m_inputStream;
}
