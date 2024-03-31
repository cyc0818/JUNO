#include <boost/python.hpp>
#include <algorithm>
#include <numeric>
#include "OrtInferenceAlg.h"
#include "SniperKernel/AlgFactory.h"

#include "TRandom.h"

DECLARE_ALGORITHM(OrtInferenceAlg);

OrtInferenceAlg::OrtInferenceAlg(const std::string& name)
    : AlgBase(name) {
    declProp("Model", m_model_file);
    declProp("IntraOpNumThreads", m_intra_op_nthreads=1);
    declProp("InterOpNumThreads", m_inter_op_nthreads=1);
}


bool OrtInferenceAlg::initialize() {

    m_env = std::make_shared<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "ENV");
    m_seesion_options = std::make_shared<Ort::SessionOptions>();
    m_seesion_options->SetIntraOpNumThreads(m_intra_op_nthreads);
    m_seesion_options->SetInterOpNumThreads(m_inter_op_nthreads);

    m_session = std::make_shared<Ort::Session>(*m_env, m_model_file.c_str(), *m_seesion_options);

    // lambda function to print the dims.
    auto dims_str = [&](const auto& dims) {
        return std::accumulate(dims.begin(), dims.end(), std::to_string(dims[0]),
                               [](const std::string& a, int64_t b){
                                   return a + "x" + std::to_string(b);
                               });
    };

    // prepare the input
    auto num_input_nodes = m_session->GetInputCount();
    LogInfo << "num_input_nodes: " << num_input_nodes << std::endl;

    for (size_t i = 0; i < num_input_nodes; ++i) {
        auto name = m_session->GetInputName(i, m_allocator);
        m_input_node_names.push_back(name);

        Ort::TypeInfo type_info  = m_session->GetInputTypeInfo(i);
        auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
        auto dims = tensor_info.GetShape();
        m_input_node_dims.push_back(dims);


        LogInfo << "[" << i << "]"
                << " name: " << name
                << " ndims: " << dims.size()
                << " dims: " << dims_str(dims)
                << std::endl;
    }

    // prepare the output
    size_t num_output_nodes = m_session->GetOutputCount();
    for(std::size_t i = 0; i < num_output_nodes; i++) {
        char* output_name = m_session->GetOutputName(i, m_allocator);
        m_output_node_names.push_back(output_name);
        Ort::TypeInfo type_info        = m_session->GetOutputTypeInfo(i);
        auto tensor_info               = type_info.GetTensorTypeAndShapeInfo();
        ONNXTensorElementDataType type = tensor_info.GetElementType();
        m_output_node_dims               = tensor_info.GetShape();
        LogInfo << "[" << i << "]"
                << " name: " << output_name
                << " ndims: " << m_output_node_dims.size()
                << " dims: " << dims_str(m_output_node_dims)
                << std::endl;

    }


    return true;
}

bool OrtInferenceAlg::execute() {

    // prepare a dummy input for the model
    std::vector<float> inputs;
    // random generate input
    for (size_t i = 0; i < m_input_node_names.size(); ++i) {
        const auto& dims = m_input_node_dims[i];

        int size = 1;
        for (auto j: dims) {
            size *= j;
        }
        LogInfo << "generating " << size << " elements. " << std::endl;
        for (size_t j = 0; j < size; ++j) {
            float v = gRandom->Uniform();
            inputs.push_back(v);
            if (j<10) LogDebug << "[" << j << "]: " << v << std::endl;
        }
    }

    const auto& dims = m_input_node_dims[0];

    Ort::MemoryInfo info("Cpu", OrtDeviceAllocator, 0, OrtMemTypeDefault);

    auto input_tensor = Ort::Value::CreateTensor(info, 
                                                 inputs.data(), 
                                                 inputs.size(), 
                                                 dims.data(), 
                                                 dims.size());
    std::vector<Ort::Value> input_tensors;
    input_tensors.push_back(std::move(input_tensor));

    auto output_tensors = m_session->Run(Ort::RunOptions{ nullptr },
                                         m_input_node_names.data(),
                                         input_tensors.data(),
                                         input_tensors.size(),
                                         m_output_node_names.data(), 
                                         m_output_node_names.size());

    for (int i = 0; i < output_tensors.size(); ++i) {
        LogInfo << "[" << i << "]"
                << " output name: " << m_output_node_names[i]
                << " results (first 10 elements): "
                << std::endl;
        const auto& output_tensor = output_tensors[i];
        const float* v_output = output_tensor.GetTensorData<float>();

        for (int j = 0; j < 10; ++j) {
            LogInfo << "[" << i << "]" << "[" << j << "] "
                    << v_output[j]
                    << std::endl;
        }
    }

    return true;
}

bool OrtInferenceAlg::finalize() {

    return true;
}

