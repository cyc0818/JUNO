# Anaconda Python

## Basic information

We provide a standalone Anaconda environment in CVMFS:

|          Anaconda Version       |    Env  | Python |                         Path                                      |
|---------------------------------|---------|--------|-------------------------------------------------------------------|
| Anaconda3-2020.11-Linux-x86_64  |   base  | 3.8.5  | /cvmfs/juno.ihep.ac.cn/sw/anaconda/Anaconda3-2020.11-Linux-x86_64 |
|                                 | root622 | 3.8.6  |                                                                   |
|                                 | root624 | 3.8.12 |                                                                   |
|                                 |   qml   | 3.8.10 |                                                                   |
|                                 | root626 | 3.9.15 |                                                                   |

To activate an Env (virtual environment), use following command:

```bash
$ source /cvmfs/juno.ihep.ac.cn/sw/anaconda/Anaconda3-2020.11-Linux-x86_64/bin/activate root622
```

If the Env does not meet your requirements, you can contact with Tao Lin <lintao@ihep.ac.cn>.

A complete version of Anaconda could be found here: https://repo.anaconda.com/archive/

## Change Log

### 07 Jan 2021: create `root622` in `Anaconda3-2020.11-Linux-x86_64`
```bash
$ conda create -n root622 python=3.8 root=6.22  -c conda-forge
$ conda install numpy scipy matplotlib -c conda-forge
$ conda install jupyter jupyterhub jupyterlab -c conda-forge
```

### 07 Jul 2021: create `qml` in `Anaconda3-2020.11-Linux-x86_64`
Here, qml means Quantum Machine Learning. 

```bash
$ conda create -n qml python=3.8
$ conda activate qml
$ pip install 'qiskit[visualization]'
$ pip install qiskit_machine_learning
$ pip install qiskit-aer-gpu
```

### 17 Feb 2022: create `root624` in `Anaconda3-2020.11-Linux-x86_64`
```bash
$ conda create -n root624 python=3.8 root=6.24 h5py numpy scipy matplotlib jupyter jupyterhub jupyterlab -c conda-forge
$ conda install uproot pandas -c conda-forge
$ conda install root_numpy -c conda-forge
$ conda install scikit-learn -c conda-forge
$ conda install 'iminuit<2' -c conda-forge
$ conda install seaborn -c conda-forge
```

### 18 Jan 2023: create `root626` in `Anaconda3-2020.11-Linux-x86_64`
Note: root_numpy is deprecated and unmaintained, so it is not installed. 

```bash
$ conda create -n root626 python=3.9 root=6.26 h5py 'iminuit<2' jupyter jupyterhub jupyterlab matplotlib numpy pandas scikit-learn scipy seaborn uproot -c conda-forge
```

