#!/usr/bin/env python
#-*- coding: utf-8 -*-
#-*- author: Tao Lin -*-

import numpy as np

##############################################################################
# Base Class Writer
##############################################################################
class BaseWriter(object):

    def save(self, data):
        # Note: data should be a dict
        raise NotImplementedError("Need to implement save in the derived classes")

##############################################################################
# Stdout Writer
##############################################################################
class StdoutWriter(BaseWriter):

    def save(self, data):
        print(data)

##############################################################################
# Numpy npz Writer
##############################################################################
class NumpyWriter(BaseWriter):

    def __init__(self, fn):
        self.fn = fn

    def save(self, data):
        import numpy as np
        np.savez(self.fn, **data)

##############################################################################
# HDF5 Writer
##############################################################################
class HDF5Writer(BaseWriter):

    def __init__(self, fn):
        self.fn = fn

    def save(self, data):
        import h5py
        with h5py.File(self.fn, "w") as f:
            for k,v in data.items():
                # print(k)
                # print(v)
                # print(v.dtype)
                f.create_dataset(k, data=v)

##############################################################################
# ROOT Writer
##############################################################################
class ROOTWriter(BaseWriter):

    def __init__(self, fn):
        self.fn = fn

    def save(self, data):
        import ROOT

        f = ROOT.TFile.Open(self.fn, "RECREATE")

        for name, arr in data.items():
            self.convert_to_tree(name, arr)

        f.Close()

    def convert_to_tree(self, name, arr):
        import ROOT
        from array import array

        tree = ROOT.TTree(name, name)

        dtypes = arr.dtype
        fields = dtypes.fields

        one_record = []
        one_type = []

        for field_name in dtypes.names:
            dtype, offset = fields[field_name]
            t = None # the array type
            v = None # the array value
            type_in_root = None

            if dtype.char in ('i', 'I', 'l', 'L'):
                t = 'L'
                v = [0]
                type_in_root = "%s/L"%field_name
            elif dtype.char == 'S':
                t = 'B'
                v = [0]*dtype.itemsize
                type_in_root = "%s[%d]/C"%(field_name, dtype.itemsize)
            else:
                print("dtype %s need to register. "%str(dtype))
                print("dtype.char: %s"%dtype.char)

            col = array(t, v)
            one_record.append(col)
            one_type.append(dtype)

            tree.Branch(field_name, 
                        one_record[-1], 
                        type_in_root)

        for r in arr:
            # fill each col into one_record

            for i,v in enumerate(r):
                dtype = one_type[i]
                col = one_record[i]
                if dtype.char in ('i', 'I', 'l', 'L'):
                    col[0] = v
                elif dtype.char == 'S':
                    # reset
                    for ii in range(dtype.itemsize):
                        col[ii] = 0
                    for ii,cc in enumerate(v):
                        col[ii] = cc
                else:
                    print("dtype %s need to register. "%str(dtype))
                    print("dtype.char: %s"%dtype.char)

            tree.Fill()

        tree.Write()
        

##############################################################################
# SQLite3 Writer
##############################################################################
class SQLite3Writer(BaseWriter):

    TABLE_GlobalTag_CREATE = '''
    CREATE TABLE GlobalTag(
        cond_gtag_name TEXT
    )
    '''

    TABLE_GlobalTag_INSERT = '''
    INSERT INTO GlobalTag(cond_gtag_name)
                  VALUES(?)
    '''

    TABLE_CondGTag_CREATE = '''
    CREATE TABLE CondGTag(
        id INTEGER PRIMARY KEY,
        name TEXT
    )
    '''

    TABLE_CondGTag_INSERT = '''
    INSERT INTO CondGTag(id, name)
                  VALUES(?,  ?)
    '''

    TABLE_CondGTag2Tag_CREATE = '''
    CREATE TABLE CondGTag2Tag(
        cond_gtag_id INTEGER,
        cond_gtag_name TEXT,
        tag_id INTEGER,
        tag_name TEXT,
        object_type TEXT
    )
    '''

    TABLE_CondGTag2Tag_INSERT = '''
    INSERT INTO CondGTag2Tag(cond_gtag_id, cond_gtag_name, tag_id, tag_name, object_type)
                      VALUES(?,            ?,              ?,      ?,        ?)
    '''


    TABLE_CondTag_CREATE = '''
    CREATE TABLE CondTag(
        id INTEGER PRIMARY KEY,
        name TEXT,
        object_type TEXT
    )
    '''

    TABLE_CondTag_INSERT = '''
    INSERT INTO CondTag(id, name, object_type)
                 VALUES(?,  ?,    ?)
    '''

    TABLE_CondTag2IOV_CREATE = '''
    CREATE TABLE CondTag2IOV(
        tag_name TEXT,
        tag_id INTEGER,
        iov_id INTEGER
    )
    '''

    TABLE_CondTag2IOV_INSERT = '''
    INSERT INTO CondTag2IOV(tag_name, tag_id, iov_id)
                     VALUES(?,        ?,      ?)
    '''

    TABLE_CondIOV_CREATE = '''
    CREATE TABLE CondIOV(
        id INTEGER PRIMARY KEY,
        since INTEGER,
        payload_hash TEXT,
        object_type TEXT
    )
    '''

    TABLE_CondIOV_INSERT = '''
    INSERT INTO CondIOV(id, since, payload_hash, object_type)
                 VALUES(?,  ?,     ?,            ?)
    '''

    TABLE_CondPayload_CREATE = '''
    CREATE TABLE CondPayload(
        id INTEGER PRIMARY KEY,
        hash TEXT,
        object_type TEXT,
        path TEXT,
        data BLOB,
        streamer_info BLOB
    )
    '''

    TABLE_CondPayload_INSERT = '''
    INSERT INTO CondPayload(id, hash, object_type, path)
                     VALUES(?,  ?,    ?,           ?)
    '''


    ###########################################################################
    # ParaDB
    ###########################################################################

    TABLE_ParaGTag_CREATE = '''
    CREATE TABLE ParaGTag(
        id INTEGER PRIMARY KEY,
        name TEXT
    )
    '''

    TABLE_ParaGTag_INSERT = '''
    INSERT INTO ParaGTag(id, name)
                  VALUES(?,  ?)
    '''

    TABLE_ParaGTag2Tag_CREATE = '''
    CREATE TABLE ParaGTag2Tag(
        para_gtag_id INTEGER,
        para_gtag_name TEXT,
        tag_id INTEGER,
        tag_name TEXT,
        object_type TEXT
    )
    '''

    TABLE_ParaGTag2Tag_INSERT = '''
    INSERT INTO ParaGTag2Tag(para_gtag_id, para_gtag_name, tag_id, tag_name, object_type)
                      VALUES(?,            ?,              ?,      ?,        ?)
    '''

    TABLE_ParaTag_CREATE = '''
    CREATE TABLE ParaTag(
        id INTEGER PRIMARY KEY,
        name TEXT,
        object_type TEXT
    )
    '''

    TABLE_ParaTag_INSERT = '''
    INSERT INTO ParaTag(id, name, object_type)
                 VALUES(?,  ?,    ?)
    '''

    TABLE_ParaTag2Payload_CREATE = '''
    CREATE TABLE ParaTag2Payload(
        tag_name TEXT,
        tag_id INTEGER,
        payload_id INTEGER
    )
    '''

    TABLE_ParaTag2Payload_INSERT = '''
    INSERT INTO ParaTag2Payload(tag_name, tag_id, payload_id)
                         VALUES(?,        ?,      ?)
    '''

    TABLE_ParaPayload_CREATE = '''
    CREATE TABLE ParaPayload(
        id INTEGER PRIMARY KEY,
        object_type TEXT,
        property TEXT,
        file_system TEXT,
        path TEXT,
        version TEXT
    )
    '''

    TABLE_ParaPayload_INSERT = '''
    INSERT INTO ParaPayload(id, object_type, property, file_system, path, version)
                     VALUES(?,  ?,           ?,        ?,           ?,    ?)
    '''


    def __init__(self, fn, t):
        self.fn = fn
        self.type = t
        self.con = None # DB connection

    def save(self, data):
        import sqlite3

        self.con = sqlite3.connect(self.fn)

        # if it is CondDB
        if self.type == "Cond":
            self.saveCondDB(data)
        elif self.type == "Para":
            self.saveParaDB(data)
        else:
            print("Unsupport DB %s"%self.type)

    def saveCondDB(self, data):

        globaltags = data["globaltags"]
        tags = data["tags"]
        iovs = data["iovs"]
        payloads = data["payloads"]

        # table GlobalTag
        gtags_ = globaltags[['gtag_name']]
        gtags_ = map(lambda x: (x[0].decode(),), gtags_)
        self.con.execute(self.TABLE_GlobalTag_CREATE)
        self.con.executemany(self.TABLE_GlobalTag_INSERT, gtags_)
        self.con.commit()

        # table CondGTag
        gtags_ = globaltags[['gtag_id', 'gtag_name']]
        gtags_ = map(lambda x: (int(x[0]), x[1].decode()), gtags_)
        self.con.execute(self.TABLE_CondGTag_CREATE)
        self.con.executemany(self.TABLE_CondGTag_INSERT, gtags_)
        self.con.commit()

        # table CondGTag2Tag
        tags_ = tags[['gtag_id', 'gtag_name', 'tag_id', 'tag_name', 'object_type']]
        tags_ = map(lambda x: (int(x[0]), x[1].decode(), int(x[2]), x[3].decode(), x[4].decode()), tags_)
        self.con.execute(self.TABLE_CondGTag2Tag_CREATE)
        self.con.executemany(self.TABLE_CondGTag2Tag_INSERT, tags_)
        self.con.commit()

        # table CondTag
        tags_ = tags[['tag_id', 'tag_name', 'object_type']]
        tags_ = map(lambda x: (int(x[0]), x[1].decode(), x[2].decode()), tags_)
        self.con.execute(self.TABLE_CondTag_CREATE)
        self.con.executemany(self.TABLE_CondTag_INSERT, tags_)
        self.con.commit()

        # table CondTag2IOV
        iovs_ = iovs[['tag_name', 'tag_id', 'iov_id']]
        iovs_ = map(lambda x: (x[0].decode(), int(x[1]), int(x[2])), iovs_)
        self.con.execute(self.TABLE_CondTag2IOV_CREATE)
        self.con.executemany(self.TABLE_CondTag2IOV_INSERT, iovs_)
        self.con.commit()

        # table CondIOV
        iovs_ = iovs[['iov_id', 'since', 'payload_hash', 'object_type']]
        iovs_ = map(lambda x: (int(x[0]), int(x[1]), x[2].decode(), x[3].decode()), iovs_)
        self.con.execute(self.TABLE_CondIOV_CREATE)
        self.con.executemany(self.TABLE_CondIOV_INSERT, iovs_)
        self.con.commit()

        # table CondPayload
        payloads_ = payloads[['payload_id', 'payload_hash', 'object_type', 'path']]
        payloads_ = map(lambda x: (int(x[0]), x[1].decode(), x[2].decode(), x[3].decode()), payloads_)
        self.con.execute(self.TABLE_CondPayload_CREATE)
        self.con.executemany(self.TABLE_CondPayload_INSERT, payloads_)
        self.con.commit()
        pass

    def saveParaDB(self, data):

        globaltags = data["globaltags"]
        tags = data["tags"]
        payloads = data["payloads"]

        # table ParaGTag
        gtags_ = globaltags[['gtag_id', 'gtag_name']]
        gtags_ = map(lambda x: (int(x[0]), x[1].decode()), gtags_)
        self.con.execute(self.TABLE_ParaGTag_CREATE)
        self.con.executemany(self.TABLE_ParaGTag_INSERT, gtags_)
        self.con.commit()

        # table ParaGTag2Tag
        tags_ = tags[['gtag_id', 'gtag_name', 'tag_id', 'tag_name', 'object_type']]
        tags_ = map(lambda x: (int(x[0]), x[1].decode(), int(x[2]), x[3].decode(), x[4].decode()), tags_)
        self.con.execute(self.TABLE_ParaGTag2Tag_CREATE)
        self.con.executemany(self.TABLE_ParaGTag2Tag_INSERT, tags_)
        self.con.commit()

        # table ParaTag
        tags_ = tags[['tag_id', 'tag_name', 'object_type']]
        tags_ = map(lambda x: (int(x[0]), x[1].decode(), x[2].decode()), tags_)
        self.con.execute(self.TABLE_ParaTag_CREATE)
        self.con.executemany(self.TABLE_ParaTag_INSERT, tags_)
        self.con.commit()

        # table ParaTag2Payload
        payloads_ = payloads[['tag_name', 'tag_id', 'payload_id']]
        payloads_ = map(lambda x: (x[0].decode(), int(x[1]), int(x[2])), payloads_)
        self.con.execute(self.TABLE_ParaTag2Payload_CREATE)
        self.con.executemany(self.TABLE_ParaTag2Payload_INSERT, payloads_)
        self.con.commit()

        # table ParaPayload
        payloads_ = payloads[['payload_id', 'object_type', 'property', 'file_system', 'path', 'version']]
        payloads_ = map(lambda x: (int(x[0]), x[1].decode(), x[2].decode(), x[3].decode(),
                                   x[4].decode(), x[5].decode()), payloads_)
        self.con.execute(self.TABLE_ParaPayload_CREATE)
        self.con.executemany(self.TABLE_ParaPayload_INSERT, payloads_)
        self.con.commit()
