#include "EDMUtil/JobInfo.h"

JobInfo::JobInfo() : m_offlineVersion("dev"), m_refCount(0)
{
}

JobInfo::~JobInfo()
{
}

const std::string& JobInfo::getJobOption() const
{
    return m_jobOption;
}

const std::string& JobInfo::getOfflineVersion() const
{
    return m_offlineVersion;
}

const std::string& JobInfo::getJobOptionJson() const
{
    return m_jobOptionJson;
}

const std::string& JobInfo::getHostName() const
{
    return m_hostname;
}

const Long_t& JobInfo::getStartingTime() const
{
    return m_startingTime;
}

void JobInfo::setJobOption(const std::string& value)
{
    m_jobOption = value;
}

void JobInfo::setOfflineVersion(const std::string& value)
{
    m_offlineVersion = value;
}

void JobInfo::setJobOptionJson(const std::string& value)
{
    m_jobOptionJson = value;
}

void JobInfo::setHostName(const std::string& value)
{
    m_hostname = value;
}

void JobInfo::setStartingTime(const Long_t& value)
{
    m_startingTime = value;
}

void JobInfo::addRef()
{
    ++m_refCount;
}

void JobInfo::decRef()
{
    if (--m_refCount <= 0) {
        delete this;
    }
}
