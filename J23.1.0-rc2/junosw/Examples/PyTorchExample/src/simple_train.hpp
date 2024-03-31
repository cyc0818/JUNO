#ifndef simple_train_hpp
#define simple_train_hpp

#include "simple_network.hpp"

template <typename DataLoader>
void train(
           int32_t epoch,
           Net& model,
           torch::Device device,
           DataLoader& data_loader,
           torch::optim::Optimizer& optimizer,
           size_t dataset_size) {
    model.train();
    size_t batch_idx = 0;
    for (auto& batch : data_loader) {
        auto data = batch.data.to(device), targets = batch.target.to(device);
        optimizer.zero_grad();
        auto output = model.forward(data);
        auto loss = torch::nll_loss(output, targets);
        AT_ASSERT(!std::isnan(loss.template item<float>()));
        loss.backward();
        optimizer.step();

        if (batch_idx++ % kLogInterval == 0) {
            std::printf(
                        "\rTrain Epoch: %ld [%5ld/%5ld] Loss: %.4f",
                        epoch,
                        batch_idx * batch.data.size(0),
                        dataset_size,
                        loss.template item<float>());
        }
    }
}


#endif
