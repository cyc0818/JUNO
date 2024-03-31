
#include <boost/python.hpp>
#include "PyTorchExampleAlg.h"
#include "SniperKernel/AlgFactory.h"

#include "simple_network.hpp"
#include "simple_train.hpp"
#include "simple_test.hpp"

#include <cstdlib>
#include <fstream>

DECLARE_ALGORITHM(PyTorchExampleAlg);


PyTorchExampleAlg::PyTorchExampleAlg(const std::string& name)
    : AlgBase(name), m_device(nullptr), m_model(nullptr) {
    declProp("mnist", m_mnist_path);
}

bool PyTorchExampleAlg::initialize() {
    LogInfo << "Loading MNIST data " << m_mnist_path << std::endl;

    std::ifstream f(m_mnist_path, std::ios::binary);
    if (f.fail() || !f.is_open()) {
        LogError << "Failed to load " << m_mnist_path << std::endl;
        return false;
    }

    if (f.seekg(0, std::ios::end).fail()) {
        return false;
    }
    f.close();

    
    if (torch::cuda::is_available()) {
        LogInfo << "CUDA available! Training on GPU." << std::endl;
        device_type = torch::kCUDA;
    } else {
        LogInfo << "Training on CPU." << std::endl;
        device_type = torch::kCPU;
    }

    m_device = new torch::Device(device_type);

    m_model = new Net;
    m_model->to(*m_device);

    auto train_dataset = torch::data::datasets::MNIST(m_mnist_path)
        .map(torch::data::transforms::Normalize<>(0.1307, 0.3081))
        .map(torch::data::transforms::Stack<>());

    auto train_dataset_size = train_dataset.size().value();
    auto train_loader =
        torch::data::make_data_loader<
            torch::data::samplers::SequentialSampler>(
                                                      std::move(train_dataset), kTrainBatchSize);


    auto test_dataset = torch::data::datasets::MNIST(
                                                m_mnist_path,
                                                torch::data::datasets::MNIST::Mode::kTest)
        .map(torch::data::transforms::Normalize<>(0.1307, 0.3081))
        .map(torch::data::transforms::Stack<>());
    auto test_dataset_size = test_dataset.size().value();
    auto test_loader =
        torch::data::make_data_loader(std::move(test_dataset), kTestBatchSize);

    torch::optim::SGD optimizer(
                                m_model->parameters(), torch::optim::SGDOptions(0.01).momentum(0.5));

    for (size_t epoch = 1; epoch <= kNumberOfEpochs; ++epoch) {
        train(epoch, *m_model, *m_device, *train_loader, optimizer, train_dataset_size);
        test(*m_model, *m_device, *test_loader, test_dataset_size);
    }

    return true;
}

bool PyTorchExampleAlg::execute() {

    return true;
}

bool PyTorchExampleAlg::finalize() {

    return true;
}

