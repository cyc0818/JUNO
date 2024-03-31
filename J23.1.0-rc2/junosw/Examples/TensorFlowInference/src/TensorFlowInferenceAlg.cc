
#include <boost/python.hpp>
#include "TensorFlowInferenceAlg.h"
#include "SniperKernel/AlgFactory.h"

#include <cstdlib>
#include <fstream>

DECLARE_ALGORITHM(TensorFlowInferenceAlg);


TensorFlowInferenceAlg::TensorFlowInferenceAlg(const std::string& name)
    : AlgBase(name), m_buffer(nullptr), m_graph(nullptr) {
    declProp("model", m_model_path);
}

bool TensorFlowInferenceAlg::initialize() {
    printf("Hello from TensorFlow C library version %s\n", TF_Version());

    m_buffer = read_buffer_from_file(m_model_path);
    m_graph = TF_NewGraph();

    TF_Status* status = TF_NewStatus();
    TF_ImportGraphDefOptions* opts = TF_NewImportGraphDefOptions();

    TF_GraphImportGraphDef(m_graph, m_buffer, opts, status);

    if (TF_GetCode(status) != TF_OK) {
        LogError << "Can't import GraphDef" << std::endl;
        return false;
    }
    
    LogInfo << "Load Graph " << m_model_path << std::endl;
    

    return true;
}

bool TensorFlowInferenceAlg::execute() {

    return true;
}

bool TensorFlowInferenceAlg::finalize() {

    return true;
}

static void DeallocateBuffer(void* data, size_t) {
    std::free(data);
}

TF_Buffer* TensorFlowInferenceAlg::read_buffer_from_file(const std::string& filename) {
    std::ifstream f(filename, std::ios::binary);

    if (f.fail() || !f.is_open()) {
        return nullptr;
    }

    if (f.seekg(0, std::ios::end).fail()) {
        return nullptr;
    }
    auto fsize = f.tellg();
    if (f.seekg(0, std::ios::beg).fail()) {
        return nullptr;
    }

    if (fsize <= 0) {
        return nullptr;
    }

    auto data = static_cast<char*>(std::malloc(fsize));
    if (f.read(data, fsize).fail()) {
        return nullptr;
    }
    
    f.close();


    TF_Buffer* buf = TF_NewBuffer();

    buf->data = data;
    buf->length = fsize;
    buf->data_deallocator = DeallocateBuffer;

    return buf;
}
