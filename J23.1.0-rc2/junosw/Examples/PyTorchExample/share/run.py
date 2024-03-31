#!/usr/bin/python

MNIST_DATASET = "data"

# Before running, need to download MNIST dataset
def prepare_mnist(mnist_path):
    import os
    import os.path
    if os.path.exists(mnist_path):
        return

    cmd = "python download_mnist.py -d %s"%mnist_path
    os.system(cmd)

prepare_mnist(MNIST_DATASET)

import Sniper
task = Sniper.Task("task")
task.setLogLevel(3)

Sniper.loadDll("libPyTorchExample.so")
alg = task.createAlg("PyTorchExampleAlg/alg_example")
alg.property("mnist").set(MNIST_DATASET)
task.setEvtMax(10)
task.show()
task.run()
