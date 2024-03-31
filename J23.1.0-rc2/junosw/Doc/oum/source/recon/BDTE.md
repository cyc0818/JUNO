# Reconstruction using BDTE

BDTE is an energy reconstruction algorthim using Boosted Decision Tree (BDT) optimized for positron energy reconstruction. It is described in [this paper](https://arxiv.org/abs/2206.09040).

## Usage

The common way to use the algorithm is through the `tut_calib2rec.sh` script located in `$TUTORIALROOT/share`.

```
$> $TUTORIALROOT/share/tut_calib2rec.sh (common options) --method bdte [--model modelPath]
```

Where `modelPath` is a path to a saved model. Two trained model are available: 
 - `$JUNOTOP/data/Reconstruction/BDTE/xgb_opt_Qedep_flat.json` : Reconstructing Qedep
 - `$JUNOTOP/data/Reconstruction/BDTE/xgb_opt_edep_flat.json` : Reconstructing edep

It does not produce yet anything in the user output but will write a new reconstructed vertex in the EDM.
Because this algortihm only reconstruct deposited energy, only the [`JM::RecVertex::energy()`](https://juno.ihep.ac.cn/~offline/Doxygen/edm/html/classJM_1_1RecVertex.html) field will be non-zero

**WARNING** : There may be some python dependecies that are missing. In that case run:
```
$> python3 -m pip install --user -r $BDTEROOT/requirements.txt
```

## Advanced usage

For more advance usage, you can refer to the script `runBDTE.sh` in `$BDTEROOT/share`

```
$> python3 $BDTEROOT/share/runBDTE.py --help
**************************************************
Welcome to SNiPER v2.1.0
Running @ basic_host on Tue Jul  4 14:32:31 2023
**************************************************
usage: runBDTE.py [-h] [--input INPUT [INPUT ...]] [--output OUTPUT] [--verbose {0,2,3,5,6}] [--mode {inference,training,control}] [--model MODEL]

Implementation of the BDTE algorithm

optional arguments:
  -h, --help            show this help message and exit
  --input INPUT [INPUT ...]
                        input file name
  --output OUTPUT       output file name
  --verbose {0,2,3,5,6}
                        verbosity
  --mode {inference,training,control}
                        Algorithm mode
  --model MODEL         Model path

**************************************************
Terminating @ bash_host on Tue Jul  4 14:32:34 2023
SNiPER::Context Running Mode = { BASIC }
SNiPER::Context Terminated Successfully
```

This script offer 3 modes:
 - **inference** : Infer the deposited energy from the inputs and write the EDM in the outputs
 - **training** : Train a model using the inputs. If model is unspecified, a new model is created using default parameters. `OUTPUT` is unused.
 - **control** : Produce the distribution of the computed features using input.  `OUTPUT` is not used. It will produce png plots and a csv containing the selected features.
 - [WIP] featureselection : Select the optimal features to train the BDT __(Not yet implemented)__

For more fine control over the training hyperparameters and the feature selection, refer to the `Config` class in `$BDTEROOT/python/BDTE/Reco.py`

For more technical informations, refer to the documentation inside the code and the `$BDTEROOT/Readme.md`
