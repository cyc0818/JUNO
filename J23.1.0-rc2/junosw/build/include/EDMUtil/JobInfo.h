#ifndef EDMUTIL_JOBINFO_H
#define EDMUTIL_JOBINFO_H

#include "TObject.h"

#include <string>

class JobInfo : public TObject {
    public:
        JobInfo();
        ~JobInfo();

        const std::string& getJobOption() const;
        const std::string& getOfflineVersion() const;
        const std::string& getJobOptionJson() const;
        const std::string& getHostName() const;
        const Long_t& getStartingTime() const;
        void setJobOption(const std::string& value);
        void setOfflineVersion(const std::string& value);
        void setJobOptionJson(const std::string& value);
        void setHostName(const std::string& value);
        void setStartingTime(const Long_t& value);
        // Reference count
        void addRef();
        void decRef();

    private:
        std::string m_jobOption;
        std::string m_offlineVersion;
        std::string m_jobOptionJson;
        std::string m_hostname;
        // /proc/self : fMtime
        Long_t m_startingTime;
        int m_refCount; //!

    ClassDef(JobInfo,2)
};

#endif
