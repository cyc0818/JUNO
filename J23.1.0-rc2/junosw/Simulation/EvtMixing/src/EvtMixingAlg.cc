#include "EvtMixingAlg.hh"

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/ToolBase.h"

#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"

#include "TRandom.h"

DECLARE_ALGORITHM(EvtMixingAlg);

EvtMixingAlg::EvtMixingAlg(const std::string& name)
    : AlgBase(name), m_time(0),
      m_buf(nullptr), m_current_time(0), 
      m_evt_counter(0), m_evt_counter_accepted(0),
      m_totalrates(0), m_totaltau(0), m_filters_enabled(false) {

    declProp("Rates", m_rates);
    declProp("Inputs", m_inputs);
    declProp("StartIdxs", m_startidxs);
    declProp("Time", m_time);

    declProp("Filters", m_premixing_filter_names);
}

bool EvtMixingAlg::initialize() {
    // GET THE DATA BUFFER
    SniperPtr<IDataMemMgr> navBuf(*getParent(), "BufferMemMgr");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    // INIT THE INPUT STREAM
    for (auto& [key, rate]: m_rates) {
        m_totalrates += rate;

        m_rates_cdf_key.push_back(key);
        m_rates_cdf.push_back(m_totalrates);

        m_inputstreams.emplace_back(key, m_inputs[key], m_startidxs);

    }

    if (m_inputstreams.size() == 0) {
        LogError << "empty input. " << std::endl;
        return false;
    }

    m_totaltau = 1./m_totalrates;

    // INIT FILTERS
    if (m_premixing_filter_names.size() > 0) {
        m_filters_enabled = true;

        for (const auto& name: m_premixing_filter_names) {
            IPreMixingFilter* pmf = tool<IPreMixingFilter>(name);
            if (not pmf) {
                LogError << "Failed to find the filter: " << name << std::endl;
                return false;
            }

            m_premixing_filters.push_back(pmf);
        }
    }

    return true;
}

bool EvtMixingAlg::execute() {

    while (true) {

        // sample the next time
        double deltatime = gRandom->Exp(m_totaltau);

        m_current_time += deltatime;

        // stop if beyond the time
        if (m_time > 0 and m_current_time > m_time) {
            dynamic_cast<Task*>(getParent())->stop(Sniper::StopRun::Peacefully);
        }

        int sec = (int)m_current_time;
        int nsec = (int)((m_current_time-sec)*1e9);
        TTimeStamp curtime(sec, nsec);

        // sample
        double r = gRandom->Uniform(m_totalrates);
        LogDebug << "sample r: " << r << std::endl;

        // get the sample (label)
        size_t sample_idx = 0;

        for (size_t i = 0; i < m_rates_cdf.size(); ++i) {
            // select the sample key
            if (r < m_rates_cdf[i]) {
                // HERE we find it
                sample_idx = i;
                break; // found it, so skip the next loop
            }
        }


        auto& current_stream = m_inputstreams[sample_idx];

        auto st = current_stream.next();

        if (not st) {
            LogError << "Failed to get the stream. " << std::endl;
            return false;
        }

        std::shared_ptr<JM::EvtNavigator> evtnav(current_stream.get());
        if (not evtnav) {
            LogError << "Failed to get event navigator. " << std::endl;
            return false;
        }


        ++m_evt_counter;

        // apply filters
        if (is_accepted(evtnav.get())) {

            // copy all to the new event navigator
            auto anew_evtnav = new JM::EvtNavigator();
            anew_evtnav->setTimeStamp(curtime);
            
            auto genhdr =  JM::getHeaderObject<JM::GenHeader>(evtnav.get());
            JM::addHeaderObject(anew_evtnav, genhdr);
            
            auto simhdr =  JM::getHeaderObject<JM::SimHeader>(evtnav.get());
            // set the tag in the simhdr
            simhdr->setEventType(m_rates_cdf_key[sample_idx]);
            JM::addHeaderObject(anew_evtnav, simhdr);

            m_buf->adopt(anew_evtnav, "/Event");

            ++m_evt_counter_accepted;

            break;
        }


    } // end of while loop

    return true;
}

bool EvtMixingAlg::finalize() {
    LogInfo << "Summary: " << std::endl;
    LogInfo << "Total Rates: " << m_totalrates << " Hz" << std::endl;
    LogInfo << "Total Tau: " << m_totaltau << " seconds" << std::endl;
    LogInfo << "Time (to be simulated): " << m_time << " seconds"  << std::endl;
    LogInfo << "Time (simulated): " << m_current_time << " seconds" << std::endl;
    LogInfo << "Number of Events: " << m_evt_counter << std::endl;
    LogInfo << "Number of Events after filter: " << m_evt_counter_accepted << std::endl;

    return true;
}

bool EvtMixingAlg::is_accepted(JM::EvtNavigator* nav) {
    // if filter is not enabled, always accept
    if (not m_filters_enabled) {
        return true;
    }

    // if enabled: 
    // not accepted by default
    bool st = false;

    for (auto filter: m_premixing_filters) {
        if (filter->accept(nav)) {
            st = true;
        }
    }

    return st;

}


// ---------------------------------------------------------------------------
// below is the BranchStream
// ---------------------------------------------------------------------------

EvtMixingAlg::BranchStream::BranchStream(const std::string& label,
                                         const std::vector<std::string>& pathList,
                                         const std::map<std::string, uint64_t>& startidxmap)
    : m_label(label), m_stream(nullptr) {
    std::cout  << "Initializing BranchStream " << label
               << " with " << pathList.size() << " files." 
               << std::endl;
    m_stream = new NavInputStream(pathList);

    bool okay = m_stream->initialize();
    if (!okay) {
        std::cerr << "Failed to initialize NavInputStream " << label << std::endl;
        delete m_stream;
        m_stream = nullptr;
        return;
    }
    okay = m_stream->first(true);
    if (!okay) {
        std::cerr << "Failed to get tag stream " << label << std::endl;
        delete m_stream;
        m_stream = nullptr;
        return;
    }

    // total entry
    uint64_t entries = m_stream->getEntries();

    uint64_t startidx = 0;
    if (startidxmap.count(label)>0) {
        startidx = startidxmap.at(label);
    } else {
        startidx = static_cast<uint64_t>(entries * gRandom->Uniform());
    }
    std::cout << "Initialize startidx " << startidx
              << "/" << entries
              << " for " << label
              << std::endl;

    m_stream->setEntry(startidx);
}

bool EvtMixingAlg::BranchStream::next() {
    if (!m_stream) {
        return false;
    }

    bool st = m_stream->next();

    if (not st) { // failed to next, try reset to first
        auto cur_entry = m_stream->getEntry();
        auto total_entry = m_stream->getEntries();

        // reuse the data
        if(cur_entry+1>=total_entry){
            st = m_stream->first();
        }
    }

    return st;
}

std::shared_ptr<JM::EvtNavigator> EvtMixingAlg::BranchStream::get() {
    if (!m_stream) { return nullptr; }

    std::shared_ptr<JM::EvtNavigator> evtnav{m_stream->get()};
    return evtnav;
}

