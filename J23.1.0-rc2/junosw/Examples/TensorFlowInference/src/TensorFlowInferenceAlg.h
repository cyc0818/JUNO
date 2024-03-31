#ifndef TensorFlowInferenceAlg_h
#define TensorFlowInferenceAlg_h

/*
 * An example to run TensorFlow C API in SNiPER framework.
 *
 * The C API example: https://github.com/Neargye/hello_tf_c_api 
 *
 * Tao Lin, 29 Sept 2020
 */


#include "SniperKernel/AlgBase.h"

#include <stdio.h>
#include <tensorflow/c/c_api.h>

class TensorFlowInferenceAlg: public AlgBase {
public:
    TensorFlowInferenceAlg(const std::string& name);

    bool initialize();
    bool execute();
    bool finalize();

private:
    TF_Buffer* read_buffer_from_file(const std::string& filename);

private:
    TF_Buffer* m_buffer;
    TF_Graph* m_graph;

private:
    std::string m_model_path;

};

#endif
