#!/usr/bin/env python
#-*- coding: utf-8 -*-
#-*- author: Tao Lin -*-

import numpy as np

from .models import CondDBModels, ParaDBModels

##############################################################################
# Base Class Loader
##############################################################################
class BaseLoader(object):

    def load(self):
        raise NotImplementedError("Need to implement load in the derived classes")

##############################################################################
# CondDB Dummy Loader
##############################################################################

class CondDBDummyLoader(BaseLoader):

    def load(self):
        results = {}

        results["globaltags"] = self.create_globaltags()
        results["tags"] = self.create_tags()
        results["iovs"] = self.create_iovs()
        results["payloads"] = self.create_payloads()

        return results

    def create_globaltags(self):

        gtags = np.array([(1,"J22.1.0")], 
                         dtype=CondDBModels.globaltags_dtype)
        return gtags

    def create_tags(self):

        tags = np.array([
            (1, "Calib.Deconv.AvgSpeWave.J22.1.0-rc2.V1", "Calib.Deconv.AvgSpeWave", 1, "J22.1.0"),
            (2, "Calib.Deconv.Filter.J22.1.0-rc2.V1",     "Calib.Deconv.Filter",     1, "J22.1.0"),
            (3, "Calib.Deconv.Para.J22.1.0-rc2.V1",       "Calib.Deconv.Para",       1, "J22.1.0"),
            (4, "Calib.Deconv.SPEs.J22.1.0-rc2.V1",       "Calib.Deconv.SPEs",       1, "J22.1.0"),
        ], dtype=CondDBModels.tags_dtype)
        return tags

    def create_iovs(self):

        iovs = np.array([
            (1, 20220704, "f0419abc2ed298dd26284167a0f8a3e8", "Calib.Deconv.SPEs",       4, "Calib.Deconv.SPEs.J22.1.0-rc2.V1",       1, "J22.1.0"),
            (2, 20220704, "c5f2c4d3b9383361ab3a6c5abdaa0e33", "Calib.Deconv.Para",       3, "Calib.Deconv.Para.J22.1.0-rc2.V1",       1, "J22.1.0"),
            (3, 20220704, "04900e2105fb745d8b5fdbe1f313f1a7", "Calib.Deconv.AvgSpeWave", 1, "Calib.Deconv.AvgSpeWave.J22.1.0-rc2.V1", 1, "J22.1.0"),
            (4, 20220704, "b26cecbd8dc13bc013f76a329afab80d", "Calib.Deconv.Filter",     2, "Calib.Deconv.Filter.J22.1.0-rc2.V1",     1, "J22.1.0"),
        ], dtype=CondDBModels.iovs_dtype)

        return iovs

    def create_payloads(self):

        payloads = np.array([
            (1, "04900e2105fb745d8b5fdbe1f313f1a7", "Calib.Deconv.AvgSpeWave",
             "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc2/data/Reconstruction/Deconvolution/share/SPE_v20.root",
             3, 20220704, 
             1, "Calib.Deconv.AvgSpeWave.J22.1.0-rc2.V1",
             1, "J22.1.0"),
            (2, "f0419abc2ed298dd26284167a0f8a3e8", "Calib.Deconv.SPEs",
             "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc2/data/Calibration/PMTCalibSvc/data/output_deconv.root",
             1, 20220704, 
             4, "Calib.Deconv.SPEs.J22.1.0-rc2.V1",
             1, "J22.1.0"),
            (3, "b26cecbd8dc13bc013f76a329afab80d", "Calib.Deconv.Filter",
             "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc2/data/Reconstruction/Deconvolution/share/filter3_m.root",
             4, 20220704,
             2, "Calib.Deconv.Filter.J22.1.0-rc2.V1",
             1, "J22.1.0"),
            (4, "c5f2c4d3b9383361ab3a6c5abdaa0e33", "Calib.Deconv.Para",
             "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc2/data/Calibration/PMTCalibSvc/data/PmtPrtData_deconv.txt",
             2, 20220704,
             3, "Calib.Deconv.Para.J22.1.0-rc2.V1",
             1, "J22.1.0"),
        ], dtype=CondDBModels.payloads_dtype)
        
        return payloads

##############################################################################
# CondDB MySQL Loader
##############################################################################

class CondDBMySQLLoader(BaseLoader):

    QUERY_GLOBALTAGS = """
    SELECT id, name from OfflineDB.CondGTag where name=%s
    """

    QUERY_TAGS = """
    SELECT tag_id, tag_name, object_type, cond_gtag_id, cond_gtag_name
    from OfflineDB.CondGTag2Tag where cond_gtag_name=%s
    """

    QUERY_IOVS = """
    SELECT tiov.iov_id, iov.since, iov.payload_hash, iov.object_type, 
           tim.tag_id, tim.tag_name, tim.cond_gtag_id, tim.cond_gtag_name
    FROM OfflineDB.CondTag2IOV tiov
    INNER JOIN (
        SELECT tag_id, tag_name, cond_gtag_id, cond_gtag_name
        FROM OfflineDB.CondGTag2Tag where cond_gtag_name=%s
    ) tim ON tim.tag_id = tiov.tag_id
    INNER JOIN OfflineDB.CondIOV iov ON iov.id = tiov.iov_id
    """

    QUERY_PAYLOADS = """
    SELECT payload.id, payload.hash, payload.object_type, payload.path,
           iov.id, iov.since,
           tim.tag_id, tim.tag_name, tim.cond_gtag_id, tim.cond_gtag_name
    FROM OfflineDB.CondTag2IOV tiov
    INNER JOIN (
        SELECT tag_id, tag_name, cond_gtag_id, cond_gtag_name
        FROM OfflineDB.CondGTag2Tag where cond_gtag_name=%s
    ) tim ON tim.tag_id = tiov.tag_id
    INNER JOIN OfflineDB.CondIOV iov ON iov.id = tiov.iov_id
    INNER JOIN OfflineDB.CondPayload payload ON iov.payload_hash = payload.hash
    """


    def __init__(self, cond_gtag_name="J22.1.0"):
        import pymysql
        import pymysql.cursors
        import os
        
        conf_path = os.path.join(os.getenv("HOME"), ".my.cnf")
        self.conn = pymysql.connect(read_default_file=conf_path, read_default_group='client')

        self.cond_gtag_name = cond_gtag_name

    def load(self):
        results = {}

        results["globaltags"] = self.create_globaltags()
        results["tags"] = self.create_tags()
        results["iovs"] = self.create_iovs()
        results["payloads"] = self.create_payloads()

        return results

    def create_globaltags(self):

        gtags = []

        with self.conn.cursor() as cursor:

            n = cursor.execute(self.QUERY_GLOBALTAGS, (self.cond_gtag_name,))
            if n == 0:
                print("WARNING: Get empty result for SQL: %s"%self.QUERY_GLOBALTAGS)

            for r in cursor.fetchall():
                gtags.append(r)
        
        return np.array(gtags, dtype=CondDBModels.globaltags_dtype)

    def create_tags(self):

        tags = []

        with self.conn.cursor() as cursor:

            n = cursor.execute(self.QUERY_TAGS, (self.cond_gtag_name,))
            if n == 0:
                print("WARNING: Get empty result for SQL: %s"%self.QUERY_TAGS)

            for r in cursor.fetchall():
                tags.append(r)
        
        return np.array(tags, dtype=CondDBModels.tags_dtype)

        pass

    def create_iovs(self):
        iovs = []

        with self.conn.cursor() as cursor:

            n = cursor.execute(self.QUERY_IOVS, (self.cond_gtag_name,))
            if n == 0:
                print("WARNING: Get empty result for SQL: %s"%self.QUERY_IOVS)

            for r in cursor.fetchall():
                iovs.append(r)
        
        return np.array(iovs, dtype=CondDBModels.iovs_dtype)

        pass

    def create_payloads(self):
        payloads = []

        with self.conn.cursor() as cursor:

            n = cursor.execute(self.QUERY_PAYLOADS, (self.cond_gtag_name,))
            if n == 0:
                print("WARNING: Get empty result for SQL: %s"%self.QUERY_PAYLOADS)

            for r in cursor.fetchall():
                payloads.append(r)
        
        return np.array(payloads, dtype=CondDBModels.payloads_dtype)
        pass

##############################################################################
# ParaDB MySQL Loader
##############################################################################

class ParaDBMySQLLoader(BaseLoader):

    QUERY_GLOBALTAGS = """
    SELECT id, name from OfflineDB.ParaGTag where name=%s
    """

    QUERY_TAGS = """
    SELECT tag_id, tag_name, object_type, para_gtag_id, para_gtag_name
    from OfflineDB.ParaGTag2Tag where para_gtag_name=%s
    """

    QUERY_PAYLOADS = """
    SELECT payload.id, payload.object_type, payload.property, 
           payload.file_system, payload.path, payload.version,
           ttp.tag_id, ttp.tag_name, 
           gtt.para_gtag_id, gtt.para_gtag_name
    FROM OfflineDB.ParaTag2Payload ttp
    INNER JOIN OfflineDB.ParaGTag2Tag gtt ON gtt.tag_id = ttp.tag_id
    INNER JOIN OfflineDB.ParaPayload payload ON payload.id = ttp.payload_id
    WHERE gtt.para_gtag_name=%s
    """

    def __init__(self, para_gtag_name="J22.1.0"):
        import pymysql
        import pymysql.cursors
        import os
        
        conf_path = os.path.join(os.getenv("HOME"), ".my.cnf")
        self.conn = pymysql.connect(read_default_file=conf_path, read_default_group='client')

        self.para_gtag_name = para_gtag_name

    def load(self):
        results = {}

        results["globaltags"] = self.create_globaltags()
        results["tags"] = self.create_tags()
        results["payloads"] = self.create_payloads()

        return results

    def create_from_db(self, sql, numpy_dtype):
        records = []

        with self.conn.cursor() as cursor:

            n = cursor.execute(sql, (self.para_gtag_name,))
            if n == 0:
                print("WARNING: Get empty result for SQL: %s"%sql)

            for r in cursor.fetchall():
                # Note: if some fields are NULL, the results are None
                records.append(tuple(map(lambda x: x if x is not None else "" , r)))
        
        return np.array(records, dtype=numpy_dtype)

    def create_globaltags(self):
        return self.create_from_db(self.QUERY_GLOBALTAGS, ParaDBModels.globaltags_dtype)

    def create_tags(self):
        return self.create_from_db(self.QUERY_TAGS, ParaDBModels.tags_dtype)
        pass

    def create_payloads(self):
        return self.create_from_db(self.QUERY_PAYLOADS, ParaDBModels.payloads_dtype)
        pass
