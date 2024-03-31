#ifndef OECPruneGBAlg_hh
#define OECPruneGBAlg_hh

/*
 * Description:
 *
 *     OECPruneGBAlg is used in the multi-threaded OEC (offline).
 *     The original version is OECProcessorAlg. But due to using MT.SNiPER,
 *     the original algorithm is divided into:
 *     1. GlobalInput: read or produce elecevent
 *     2. WorkerTask: LEC
 *     3. GlobalOutput: HEC + Output
 *
 *     So this algorithm is implemented the 3rd task in GlobalOutput.
 *
 *     In OECProcessorAlg, it contains three steps. 
 *
 *     1. Fill the events from Global Buffer. 
 *        In order to reuse the existing PruneGlobalBuffer algorithm, 
 *        the PruneGlobalBuffer will be invoked in a sub-task. 
 *
 *     2. Invoke the HEC task when the events are enough.
 *
 *     3. Invoke the output task
 *
 * Author:
 *     Tao Lin <lintao@ihep.ac.cn>
 */

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/Task.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "OECProcessor/OECBuffer.h"
#include "OECTagSvc/OECTagSvc.h"

#include "OECOutputHelper.hh"

class OECPruneGBAlg: public AlgBase {
public:
    OECPruneGBAlg(const std::string& name);
    ~OECPruneGBAlg();

    bool initialize();
    bool execute();
    bool finalize();

private:
    bool writeOutput();

private:
    //The buffers
    OECBuffer *m_buf;
    JM::NavBuffer* m_navbuf_input;
    IDataMemMgr* m_memmgr_lec;

    OECTagSvc* m_tagsvc;
    OECOutputHelper* m_output_helper;

private:
    std::string m_taskname_input;
    std::string m_taskname_hec;
    std::string m_taskname_output;

    Task* m_task_input;  // let PruneGlobalBuffer to put events into data buffer
    Task* m_task_hec;
    Task* m_task_output;

    //constant parameters
    std::vector<double> v_bufRange;

};

#endif
