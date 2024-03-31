#ifndef OrtInferenceAlg_h
#define OrtInferenceAlg_h

/*
 * Description:
 *   An example to run ONNX Runtime in SNiPER.
 *
 * Author: 
 *   Tao Lin, 26 May 2022
 */

#include "SniperKernel/AlgBase.h"
#include "core/session/onnxruntime_cxx_api.h"

class OrtInferenceAlg: public AlgBase {
public:
    OrtInferenceAlg(const std::string& name);

    bool initialize();
    bool execute();
    bool finalize();

private:
    std::string m_model_file;
    int m_intra_op_nthreads;
    int m_inter_op_nthreads;

private:
    std::shared_ptr<Ort::Env> m_env;
    std::shared_ptr<Ort::SessionOptions> m_seesion_options;
    std::shared_ptr<Ort::Session> m_session;
    Ort::AllocatorWithDefaultOptions m_allocator;

private:
    std::vector<const char*> m_input_node_names;
    std::vector<std::vector<int64_t>> m_input_node_dims;
    std::vector<const char*> m_output_node_names;
    std::vector<int64_t> m_output_node_dims;

};

#endif
