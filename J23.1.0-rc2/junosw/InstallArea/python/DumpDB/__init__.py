#!/usr/bin/env python
#-*- coding: utf-8 -*-
#-*- author: Tao Lin -*-
# Description:
#   This tool is used to dump the CondDB and ParaDB. 
#
#   It consists of three layers:
#   - models: define the data model
#   - loaders: data loader
#   - writers: data writer
#
#   For simplicity, numpy is adopted to describe the corresponding models. 
#   The mapping tables are merged to the corresponding models. 
#   For example, 
#   - tags_dtype contains a column 'globaltag'
#   - iovs_dtype contains columns 'globaltag' and 'tag'
#   - payloads_dtype contains columns 'globaltag', 'tag' and 'iov'
#
#   So, the payloads are the real snapshot of all the DB
#

from .models import CondDBModels, ParaDBModels
from .loaders import CondDBDummyLoader, CondDBMySQLLoader
from .loaders import ParaDBMySQLLoader
from .writers import StdoutWriter, NumpyWriter, HDF5Writer, ROOTWriter, SQLite3Writer

def main():
    import argparse
    parser = argparse.ArgumentParser(prog=__name__,
                                     description='Dump Database into file')
    parser.add_argument('output', help='Output filename')
    parser.add_argument('-l', '--loader', default='Dummy', choices=['Dummy', 'MySQL'],
                        help="Choose the type of Loader")
    parser.add_argument('-t', '--type', default='Cond', choices=['Cond', 'Para'], 
                        help='Choose the type of database')

    args = parser.parse_args()
    print(args)

    ##########################################################################
    # Load the data
    ##########################################################################
    import sys

    loader = None
    if args.type == "Cond":
        if args.loader == 'Dummy':
            loader = CondDBDummyLoader()
        elif args.loader == 'MySQL':
            loader = CondDBMySQLLoader()
        else:
            print("Unsupport loader %s"%args.loader)
            sys.exit(-1)
    elif args.type == "Para":
        if args.loader == 'MySQL':
            loader = ParaDBMySQLLoader()
        else:
            print("Unsupport loader %s"%args.loader)
            sys.exit(-1)
    else:
        print("Unsupport type %s"%args.type)
        sys.exit(-1)

    results = loader.load()

    ##########################################################################
    # Save the data
    ##########################################################################
    writer = None
    if args.output == "stdout":
        writer = StdoutWriter()
    elif args.output.endswith(".npz"):
        writer = NumpyWriter(args.output)
    elif args.output.endswith((".h5", ".hdf5")):
        writer = HDF5Writer(args.output)
    elif args.output.endswith(".root"):
        writer = ROOTWriter(args.output)
    elif args.output.endswith(".db"):
        writer = SQLite3Writer(args.output, args.type)
    else:
        writer = StdoutWriter()

    writer.save(results)

