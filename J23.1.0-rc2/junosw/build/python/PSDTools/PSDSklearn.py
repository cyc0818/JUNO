#!/usr/bin/env python
#-*- coding: utf-8 -*-

from Sniper import PyAlgBase
import pickle
from sklearn.neural_network import MLPClassifier
import ROOT,array


##############################################################################
# Numpy is installed by default in JUNO offline software
##############################################################################
try:
    import numpy as np
except:
    np = None


class PSDSklearn(PyAlgBase):

    def __init__(self, name):
        PyAlgBase.__init__(self, name)

    def initialize(self):
        self.PSDVal_sig = array.array("d", [0])
        self.datastore = None
        self.model = None

        return True

    def execute(self):
        if self.datastore == None:
            self.datastore = self.get("DataStore").data()

        if self.model == None:
            self.load_model(self.datastore["path_model"])

        # Get Time Profile for PSD
        self.h_time_with_charge = self.access_array("h_time_with_charge")
        self.h_time_without_charge = self.access_array("h_time_without_charge")
        self.xyz_E = self.access_array("xyz_E")

        if (self.h_time_with_charge is None) or (self.h_time_without_charge is None):
            print("Error:array h_time not found!!!!! ")
            return False

        self.process_data()
        return True

    def finalize(self):
        return True

    def access_array(self, name):
        import PSDTools.PSDSklearn
        try:
            return self.datastore[name]
        except:
            print("Can't get %s in the store."%name)
            print(self.datastore)


    def process_data(self):
        # print(np.concatenate((self.h_time_without_charge/np.max(self.h_time_without_charge),
        #                       self.h_time_with_charge/np.max(self.h_time_with_charge),
        #                       [np.sum( (self.xyz_E[:3]/1000)**2)**(3/2)/17.5**3], [self.xyz_E[-1]/100])))
        self.PSDVal_sig[0] = float(self.model.predict_proba([np.concatenate((self.h_time_without_charge/np.max(self.h_time_without_charge),
                                                                    self.h_time_with_charge/np.max(self.h_time_with_charge),
                                                                    [np.sum( (self.xyz_E[:3]/1000)**2)**(3/2)/17.5**3], [self.xyz_E[-1]/100]))])[0][1])
        # self.PSDVal_sig[0] = float(self.model.predict_proba([np.concatenate((self.h_time_without_charge/np.max(self.h_time_without_charge),
        #                                                                      self.h_time_with_charge/np.max(self.h_time_with_charge)))])[0][1])
        # print("PSDVal in python:\t", self.PSDVal_sig[0])
        self.datastore["PSDVal"] = self.PSDVal_sig[0]

    def load_model(self, name_model):
        print(f"Loading Sklearn Model {name_model} .........")
        with open(name_model, "rb") as f_model:
            self.model = pickle.load(f_model)





