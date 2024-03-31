#ifndef EvtMixingAlg_hh
#define EvtMixingAlg_hh

// Description:
//   This is a utility to mix the event before electronics simulation. 
//   The motivation of this algorithm is to reduce the input for the electronics simulation. 
//   It is used to merge the radioactivity samples. 
// 
//   BranchStream is a wrapper to one type sample. It could handle two cases
//   - different parent nuclides
//   - the same parent nuclides, but different daughter nuclides in the decay chain
//
//   The second case is due to the GRDM could simulate one branch in the detsim,
//   which cause the samples are split. 
//
//   If all the branches are simulated at the same job, then the second case could be skipped. 
//
//   In order to helper user reduce the command line options, following format should be considered:
//
//       |  label   |   rate   |   filelist    |
//       |----------|----------|---------------|
//       | U238-LS  | 3.234 Hz | @U238-LS.txt  |
//       | Th232-LS | 0.733 Hz | @Th232-LS.txt |
//
//   By default, the filelist only consider the first case. 
//   In the filelist, just put all the file names in this file. 
//
//   If you need to use the second case, just keep the label same with suffix @component:
//
//       |  label              |   rate      | filelist (split or merge) |
//       |---------------------|-------------|---------------------------|
//       | U238-LS-1000822100  | r1*3.234 Hz | @U238-LS-1000822100.txt   |
//       | U238-LS-1000822140  | r2*3.234 Hz | @U238-LS-1000822140.txt   |
//       | Th232-LS            | 0.733 Hz    | @Th232-LS.txt             |
//
//    So just keep the final rate correct. 
//
// Author:
//   Tao Lin <lintao AT ihep.ac.cn>

#include <SniperKernel/AlgBase.h>

#include <EvtNavigator/NavBuffer.h>
#include <Event/GenHeader.h>
#include <Event/SimHeader.h>
#include <RootIOSvc/NavInputStream.h>
#include <BufferMemMgr/IDataMemMgr.h>

#include "IPreMixingFilter.hh"

#include <vector>
#include <map>

class EvtMixingAlg: public AlgBase {
public:

    EvtMixingAlg(const std::string& name);

    bool initialize();
    bool execute();
    bool finalize();

private:
    struct BranchStream {

        BranchStream(const std::string& label, 
                     const std::vector<std::string>& pathList,
                     const std::map<std::string, uint64_t>& startidxmap);
        
        bool next();

        std::shared_ptr<JM::EvtNavigator> get();

    private:
        std::string m_label;
        NavInputStream* m_stream;

    };

private:
    // rates: key is the label name, value is the rate
    std::map<std::string, double> m_rates;
    // file names: key is the label name, value is the input files
    std::map<std::string, std::vector<std::string>> m_inputs;
    // start idx: key is the label name, value is the start index
    //            note: if the startidx is not set, then randomize the startidx.
    std::map<std::string, uint64_t> m_startidxs;

    // running time
    // - if evtmax > 0 and time = 0, then loop will stop at evtmax.
    // - if evtmax = -1 and time > 0, then loop will stop at time.
    // - if evtmax > 0 and time > 0, then loop will stop at when it reach the evtmax or time. 
    double m_time;

private:
    IDataMemMgr* m_buf;

    double m_current_time;
    uint64_t m_evt_counter;
    uint64_t m_evt_counter_accepted;

    double m_totalrates;
    double m_totaltau; // 1./m_totalrates

    std::vector<std::string> m_rates_cdf_key;  // key: label
    std::vector<double> m_rates_cdf;           // key: label
    std::vector<BranchStream> m_inputstreams;  // key: label

private:
    std::vector<IPreMixingFilter*> m_premixing_filters;

    std::vector<std::string> m_premixing_filter_names;
    bool m_filters_enabled;

    // need to loop until got an event passed filters. 
    bool is_accepted(JM::EvtNavigator* nav);
};

#endif
