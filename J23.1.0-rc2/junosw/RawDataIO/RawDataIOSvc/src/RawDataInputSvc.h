#ifndef RAWDATA_INPUT_SVC_H
#define RAWDATA_INPUT_SVC_H

#include "RootIOSvc/IInputSvc.h"
#include "SniperKernel/SvcBase.h"

class RawDataInputSvc : public SvcBase, public IInputSvc
{
    public :

        RawDataInputSvc(const std::string& name);
        virtual ~RawDataInputSvc();

        bool initialize();
        bool finalize();

        IInputStream* getInputStream(const std::string& path);

    private :

        std::string     m_type;
        InputFileList   m_files;           // List of input files
        IInputStream*   m_inputStream;     // input stream
};

#endif
