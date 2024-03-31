#ifndef ROOT_OUTPUT_SVC_H
#define ROOT_OUTPUT_SVC_H 0

#include "SniperKernel/SvcBase.h"
#include "IOutputSvc.h"

#include <map>
#include <string>
#include <vector>

class TObject;
class RootOutputStream;
class RootOutputFileManager;

namespace JM {
    class EvtNavigator;
}

class RootOutputSvc: public IOutputSvc, public SvcBase {

    public:
        typedef std::map<std::string,std::string>     String2String;
        typedef std::multimap<std::string, TObject*>  OutputObjMap; // {path: object}
        typedef std::vector<RootOutputStream*>        OutputStreamVector;
        typedef std::vector<std::string>              StringVector;

        RootOutputSvc(const std::string& name);
        ~RootOutputSvc();
        // Service interface
        bool initialize();
        bool finalize();
        // Write current EvtNavigator and event data
        bool write(JM::EvtNavigator* nav);
        // Attach additional TObject to be written to the stream.
        bool attachObj(const std::string& path, TObject* obj);
        // Start new output file(s)
        bool newFile(const String2String& filemap);
        // Set output streams
        void setOutputStreams(const String2String& outputStream);

    private:
        bool initializeOutputStream(JM::EvtNavigator* nav);
        bool reviseOutputStream(const std::string& path, const std::string& headerName);
        bool doAttachObj(const std::string& path, TObject* obj);

    private:
        String2String          m_outputFileMap;
        String2String          m_path2typeMap;
        OutputObjMap           m_exOutputObjs;
        OutputStreamVector     m_outputStreams;
        StringVector           m_notYetInitPaths;
        bool                   m_streamInitialized;
        RootOutputFileManager* m_fileMgr;
        bool                   m_isReproducible;
};

#endif
