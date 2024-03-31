#!/usr/bin/env python
#-*- coding: utf-8 -*-
#-*- author: Tao Lin -*-

###############################################################################
#
# Note about the type
#
# | type | bytes | MySQL   | numpy |
# |------|-------|---------|-------|
# | long | 8     | bigint  | i8    |
# | str  | N     | varchar | SN    |
#
###############################################################################

import numpy as np

class CondDBModels(object):

    globaltags_dtype = np.dtype([("gtag_id", "i8"),
                                 ("gtag_name", "S64")
                                ])

    tags_dtype = np.dtype([("tag_id", "i8"),     # TAG
                           ("tag_name", "S128"), # TAG        <----|
                           ("object_type", "S128"),     # TAG      |
                           ("gtag_id", "i8"),    # GLOBAL TAG      |
                           ("gtag_name", "S64")  # GLOBAL TAG ---->|
                          ]) 

    iovs_dtype = np.dtype([("iov_id", "i8"),           # IOV <====>-<-|
                           ("since", "i8"),            # IOV       v  |
                           ("payload_hash", "S128"),   # PAYLOAD   |  |
                           ("object_type", "S128"),    # PAYLOAD <-|  |
                           ("tag_id", "i8"),           # TAG          |
                           ("tag_name", "S128"),       # TAG -------->||<-|
                           ("gtag_id", "i8"),          # GLOBAL TAG       |
                           ("gtag_name", "S64")        # GLOBAL TAG ----->|
                          ])

    payloads_dtype = np.dtype([("payload_id", "i8"),     # PAYLOAD
                               ("payload_hash", "S128"), # PAYLOAD <-|
                               ("object_type", "S128"),  # PAYLOAD   |
                               ("path", "S512"),         # PAYLOAD   |
                               ("iov_id", "i8"),         # IOV ----->||<-
                               ("since", "i8"),          # IOV          |
                               ("tag_id", "i8"),         # TAG          |
                               ("tag_name", "S128"),     # TAG -------->||<--
                               ("gtag_id", "i8"),        # GLOBAL TAG       |
                               ("gtag_name", "S64")      # GLOBAL TAG ----->|
                              ])

class ParaDBModels(object):

    globaltags_dtype = np.dtype([("gtag_id", "i8"),
                                 ("gtag_name", "S64")
                                ])

    tags_dtype = np.dtype([("tag_id", "i8"),        # TAG
                           ("tag_name", "S128"),    # TAG <-----------|
                           ("object_type", "S128"), # TAG             |
                           ("gtag_id", "i8"),       # GLOBAL TAG      |
                           ("gtag_name", "S64")     # GLOBAL TAG ---->|
                          ]) 

    payloads_dtype = np.dtype([("payload_id", "i8"),    # PAYLOAD  <---|
                               ("object_type", "S128"), # PAYLOAD      |
                               ("property", "S128"),    # PAYLOAD      |
                               ("file_system", "S64"),  # PAYLOAD      |
                               ("path", "S512"),        # PAYLOAD      |
                               ("version", "S16"),      # PAYLOAD      |
                               ("tag_id", "i8"),        # TAG          |
                               ("tag_name", "S128"),    # TAG -------->||<-
                               ("gtag_id", "i8"),       # GLOBAL TAG      |
                               ("gtag_name", "S64")     # GLOBAL TAG ---->|
                              ])
