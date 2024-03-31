#ifndef simple_test_hpp
#define simple_test_hpp

#include "simple_network.hpp"

template <typename DataLoader>
void test(
          Net& model,
          torch::Device device,
          DataLoader& data_loader,
          size_t dataset_size) {
    torch::NoGradGuard no_grad;
    model.eval();
    double test_loss = 0;
    int32_t correct = 0;
    for (const auto& batch : data_loader) {
        auto data = batch.data.to(device), targets = batch.target.to(device);
        auto output = model.forward(data);
        test_loss += torch::nll_loss(
                                     output,
                                     targets,
                                     /*weight=*/{},
                                     at::Reduction::Sum)
            .template item<float>();
        auto pred = output.argmax(1);
        correct += pred.eq(targets).sum().template item<int64_t>();
    }

    test_loss /= dataset_size;
    std::printf(
                "\nTest set: Average loss: %.4f | Accuracy: %.3f\n",
                test_loss,
                static_cast<double>(correct) / dataset_size);
}


#endif
