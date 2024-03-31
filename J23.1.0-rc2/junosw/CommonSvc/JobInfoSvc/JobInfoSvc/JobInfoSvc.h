#ifndef UTILITIES_JOBINFOSVC_H
#define UTILITIES_JOBINFOSVC_H

#include "SniperKernel/SvcBase.h"

#include <string>

class JobInfo;

class JobInfoSvc : public SvcBase {

    public:
        JobInfoSvc(const std::string& name);
        ~JobInfoSvc();

        bool initialize();
        bool finalize();

        // Get offline software of input files
        const std::string& getOfflineVersion();

    private:
        bool getJobInfo();

    private:
        JobInfo* m_iJobInfo;
        JobInfo* m_oJobInfo;

        std::string m_offline_version;
        std::string m_job_cmd;
};


#endif
