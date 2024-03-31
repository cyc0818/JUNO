#ifndef PyTorchExampleAlg_h
#define PyTorchExampleAlg_h

/*
 * An example to run PyTorch C API in SNiPER framework.
 *
 * The C API example: 
 *   - https://github.com/goldsborough/examples/tree/cpp
 *   - https://github.com/pytorch/examples/tree/master/cpp
 *
 * Tao Lin, 30 Sept 2020
 */


#include "SniperKernel/AlgBase.h"

#include <stdio.h>
#include <torch/torch.h>
#include "simple_network.hpp"

class PyTorchExampleAlg: public AlgBase {
public:
    PyTorchExampleAlg(const std::string& name);

    bool initialize();
    bool execute();
    bool finalize();

private:
    torch::DeviceType device_type;

    torch::Device* m_device;
    Net* m_model;

private:
    std::string m_mnist_path;

};

#endif
