/*  class MergingInputSvc

MergingInputSvc is a service that calls the concreate input service and merges the splited data

*/
#ifndef MERGING_INPUT_SVC_H
#define MERGING_INPUT_SVC_H 0

#include "SniperKernel/SvcBase.h"
#include "RootIOSvc/IInputStream.h"
#include "RootIOSvc/IInputSvc.h"

class TObject;
class RootInputSvc;

class MergingInputSvc: public SvcBase, public IInputSvc {

    public:
        MergingInputSvc(const std::string& name);
        ~MergingInputSvc();

        // Service interface
        bool initialize();
        bool finalize();
        IInputStream* getInputStream(const std::string& path);

    private:
        RootInputSvc* m_inputSvc;
        IInputStream* m_merger;
        std::string   m_mergingType;
        std::string   m_path;
        std::vector<std::string> m_navInputFile;
};

#endif
