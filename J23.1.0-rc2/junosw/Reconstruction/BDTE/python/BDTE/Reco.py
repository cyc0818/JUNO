from Sniper import PyAlgBase
from xgboost import XGBRegressor
import os
import pandas as pd
import numpy as np

import matplotlib.pyplot as plt

"""
AVAILABLE_FEATURES decsribe all the high order features that are available in the datastore. In details:
 - AccumCharge : Sum of the charge in every pmts
 - nPMTs: Number of fired pmts

 - *_cc : Features calculated from the charge barycenter : sum(pmt_pos * charge) / sum(charge)
 - *_cht: Features calculated from the time barycenter : sum(pmt_pos / (first_hit_time + EPSILON)) / sum(1 / (first_hit_time + EPSILON)) ; EPSILON = 50ns

 - R_{cc,cht}: Radius of the barycenter
 - rho_{cc,cht}: sqrt(x_{cc,cht} ** 2 + y_{cc,cht} ** 2)
 - {x,y,z}_{cc,cht}: coordinate of the barycenter
 - gamma_{x,y,z}_{cc,cht}: gamma of the coordinate: {x,y,z}_{cc,cht} / sqrt({y,z,x}_{cc,cht} ** 2 + {z,x,y}_{cc,cht} ** 2)
 - theta_{cc,cht} : theta angle of the barycenter in polar coordinate
 - phi_{cc,cht}: phi angle of the barycenter in polar coordinate
 - jacob_{cc,cht} : jacobian of the barycenter : R_{cc,cht} ** 2 * sin(theta_{cc,cht})

 - ht_{n}p : n percentile of the first hit time distribution
 - ht_{n}-{m}p : n percentile - m percentile of the hit time distribution
 - pe_{n}p : n percentile of the charge distribution
"""
AVAILABLE_FEATURES = ('AccumCharge', 'nPMTs',
                      'R_cc',  'rho_cc',  'x_cc',  'y_cc',  'z_cc',  'gamma_z_cc',  'gamma_y_cc',  'gamma_x_cc',  'theta_cc',  'phi_cc',  'jacob_cc',
                      'R_cht', 'rho_cht', 'x_cht', 'y_cht', 'z_cht', 'gamma_z_cht', 'gamma_y_cht', 'gamma_x_cht', 'theta_cht', 'phi_cht', 'jacob_cht',

                      'ht_std', 'ht_mean', 'ht_skew', 'ht_kurtosis',
                      'pe_std', 'pe_mean', 'pe_skew', 'pe_kurtosis',

                      'ht_2p', 'ht_5p', 'ht_10p', 'ht_15p', 'ht_20p', 'ht_25p', 'ht_30p', 'ht_35p', 'ht_40p', 'ht_45p', 'ht_50p', 'ht_55p', 'ht_60p', 'ht_65p', 'ht_70p', 'ht_75p','ht_80p', 'ht_85p', 'ht_90p', 'ht_95p',

                      'ht_5-2p', 'ht_10-5p', 'ht_15-10p', 'ht_20-15p', 'ht_25-20p', 'ht_30-25p', 'ht_35-30p', 'ht_40-35p', 'ht_45-40p', 'ht_50-45p', 'ht_55-50p', 'ht_60-55p', 'ht_65-60p', 'ht_70-65p', 'ht_75-70p', 'ht_80-75p', 'ht_85-80p', 'ht_90-85p', 'ht_95-90p',

                      'pe_2p', 'pe_5p', 'pe_10p', 'pe_15p', 'pe_20p', 'pe_25p', 'pe_30p', 'pe_35p', 'pe_40p', 'pe_45p', 'pe_50p', 'pe_55p', 'pe_60p', 'pe_65p', 'pe_70p', 'pe_75p', 'pe_80p', 'pe_85p', 'pe_90p', 'pe_95p')

"""
The default selected features
"""
SELECTED_FEATURES = ('AccumCharge', 'R_cht', 'jacob_cc', 'ht_20-15p', 'pe_std', 'nPMTs', 'z_cc', 'ht_std', 'ht_30-25p', 'R_cc', 'ht_5-2p', 'pe_mean', 'ht_25-20p', 'ht_10-5p', 'ht_35-30p')

"""
Default train target
"""
TRAIN_TARGET = 'edep'

class Config:
    """Class describing the configuration for a model

    Attributes should be directly accessed by the consumers

    Methods
    -------
    _check_param()
        Internal method to check if the parameters are coherents. Should not be used by the user

    exist()
        Check if the model already exist
    """

    def __init__(self, path: str,
                 optimal_features=AVAILABLE_FEATURES,
                 target=TRAIN_TARGET,
                 max_depth=11,
                 learning_rate=0.8,
                 n_estimators=10000,
                 FC_cut=17.2,
                 random_state=22,
                 training_fraction=0.9
                 ):
        """
        Initialize the Config object

        :param path: The path to the model or the location to save it if it do not exist yet
        :param optimal_features Tuple[str]: The features to feed to the model
        :param target {edep|qedep}: The target to train the model on
        :param max_depth int: Max depth of the BDT
        :param learning_rate float: learning rate of the BDT
        :param n_estimators int: number of estimators to use
        :param FC_cut float: radius (in meter) to use as the fiducial volume of JUNO
        :param random_state int: random seed for the initial state
        :param training_fraction float: percent of the events to use as training. Leftover will be used as validation
        """
        self.path = path
        self.optimal_features = optimal_features
        self.target = target
        self.max_depth = max_depth
        self.learning_rate = learning_rate
        self.n_estimators = n_estimators
        self.FC_cut = FC_cut
        self.random_state = random_state
        self.training_fraction = training_fraction

        self._check_param()

    def _check_param(self):
        """
        Check if the config parameters are coherent

        :raises ValueError: Raise a value error if FC_cut < 0
        :raises ValueError: Raise a value error if the training fraction is outside [0,1]
        """
        if self.FC_cut < 0: raise ValueError("[BDTConfig] FC_cut must be > 0")

        if self.training_fraction < 0 or self.training_fraction > 1: raise ValueError("[BDTConfig] training_fraction should be between 0 and 1")

    def exist(self) -> bool:
        """
        Check if the model exists
        """
        return os.path.exists(self.path)


class Mode:
    """Enumeration of the different mode
    """
    INFERENCE = 0
    TRAINING = 1
    CONTROL = 2

    @staticmethod
    def is_valid(mode: int) -> bool:
        """
        Check that the int representation of the mode is valid

        :param mode: The representation to check
        :return: True or False
        """
        return mode >= 0 and mode <= 2

class Reco(PyAlgBase):
    """Class wrapping the BDTE and implementing the method necessary to
    run in the SNiPER framework
    """

    def __init__(self, name: str, model_config: Config, mode: int = Mode.INFERENCE):
        """
        Initialize the Reco class

        :param name: Name to give to the algorithm
        :param model_config: Configuration of the BDT to use
        :param mode: Mode of the algorithm
        """
        super().__init__( name)

        self.LogDebug("Building")

        self.datastore = None
        self.accumulator = []
        self.truth_accumulator = []

        self.cur_batch = None
        self.cur_truth = None

        self.model: XGBRegressor | None = None
        self.model_config = model_config

        self.mode = mode

    def initialize(self):
        """
        Initialize the algorithm

        """
        if(not Mode.is_valid(self.mode)):
            self.LogError(f"Invalid mode {self.mode}, please give a valid mode (INFERENCE: 0, TRAINING: 1)")
            return False

        self.model = XGBRegressor(
            max_depth =     self.model_config.max_depth,
            learning_rate = self.model_config.learning_rate,
            n_estimators=   self.model_config.n_estimators,
            eval_metric= ['rmse'] if self.mode == Mode.TRAINING else None
        )

        if self.model_config.exist():
            self.model.load_model(self.model_config.path)

        self.datastore = self.get("DataStore")

        if(self.datastore):
            self.datastore = self.datastore.data()
        else:
            self.LogError("Cannot retrieve the DataStore")
            return False

        return True

    def execute(self):
        """
        Execute the algorithm

        In training and control mode, the execution just accumulate event in preparation for the trainin or control
        """

        self.LogDebug("Executing")
        acc_status = self._accumulate()
        if(not acc_status): return False
        if self.mode == Mode.INFERENCE: return self._predict()
        elif self.mode == Mode.TRAINING: return True
        elif self.mode == Mode.CONTROL: return True
        else: return False

    def finalize(self):
        """
        Finalize the algorithm

        Only usefull in case of training and controling.
        """

        self.LogDebug("Finalizing")

        if self.mode == Mode.TRAINING:
            self._train()
        if self.mode == Mode.CONTROL:
            self._control()

        return True

    def _control(self):
        """Produce a control sample of the selected features from the event in the accumulator and save the plots of the optimal_features
        """

        self.LogDebug("Controling")

        data = pd.DataFrame(self.accumulator, columns=list(self.model_config.optimal_features))
        self.accumulator = []

        for feature in self.model_config.optimal_features:
            fig = plt.figure(feature + "_fig", clear=True)
            ax = fig.subplots()
            ax.hist(data[feature], bins=50)
            ax.set_xlabel(feature)
            fig.savefig("control_" + feature + ".png")
        data.to_csv("control.csv")


    def _predict(self):
        """Predict the energy of the events in the accumulator and write the result in the datastore
        """

        self.cur_batch = pd.DataFrame(self.accumulator, columns=list(self.model_config.optimal_features))
        self.accumulator = []

        if(self.model is None):
            self.LogError("Model no yet loaded")
            return False

        prediction = self.model.predict(self.cur_batch)


        if(self.datastore is None):
            self.LogError("Datastore not defined")
            return False

        self.datastore['e_rec'] = prediction[0].item()

        return True

    def _train(self):
        """Train the model using the event in the accumulator
        """

        target_idx = "edep"

        if self.model_config.target != 'edep' and self.model_config.target != 'qedep':
            self.LogWarn("Train target is ill-defined, defaulting to targeting edep")
        else:
            target_idx = self.model_config.target


        if(self.truth_accumulator is None):
            self.LogError("Cannot train without truth. Please ensure that `edep` and `qedep` in the datastore are correctly setup")
            return False

        if(self.model is None):
            self.LogError("Model no yet loaded")
            return False

        self.cur_truth = pd.DataFrame(self.truth_accumulator, columns=["edep", "qedep"])[target_idx]
        self.cur_batch = pd.DataFrame(self.accumulator, columns=list(self.model_config.optimal_features))


        train_size = int(np.floor(len(self.truth_accumulator) * self.model_config.training_fraction))
        eval_size = int(len(self.truth_accumulator) - train_size)

        self.truth_accumulator = []
        self.accumulator = []


        X = self.cur_batch.to_numpy()[:-eval_size]
        Y = self.cur_truth.to_numpy()[:-eval_size]

        X_eval = self.cur_batch.to_numpy()[-eval_size:]
        Y_eval = self.cur_truth.to_numpy()[-eval_size:]

        self.model.fit(X, Y, xgb_model=(self.model_config.path if self.model_config.exist() else None),
                       eval_set=[(X_eval, Y_eval)],
                       verbose=self.logLevel() <= 2)

        self.model.save_model(self.model_config.path)

        prediction = self.model.predict(X_eval)

        recmtrue_err = prediction.flatten() - Y_eval.flatten()
        rmse = np.mean(recmtrue_err ** 2)
        self.LogInfo(f"Processed {train_size} ({eval_size}) events | rec-true mean: {np.mean(recmtrue_err)} | rec - true std: {np.std(recmtrue_err)} | rmse : {rmse}")


    def _accumulate(self):
        """Accumulate events in the accumulator
        """

        if(self.datastore is None):
            return False

        try:
            if self.truth_accumulator is not None:
                self.truth_accumulator.append([self.datastore['edep'], self.datastore['qedep']])
        except (KeyError, NameError):
            self.truth_accumulator = None

        self.accumulator.append([self.datastore[feature] for feature in self.model_config.optimal_features])

        return True
