#!/usr/bin/env python
#-*- coding: utf-8 -*-
#
# Description: dump the yaml format DB configuration
# Author:
#   Tao Lin <lintao AT ihep.ac.cn>
#

import yaml

def load_config(db_config_file = "dbi.yaml"):
    with open(db_config_file) as f:
        return yaml.safe_load(f)

db_config = load_config()
print(db_config)

connections = db_config.get('connections')
if connections:
    print(connections)

    for connection_name in connections:
        print(connection_name)

        connection = connections[connection_name]
        print(connection)

        # need backend name
        backend = connection.get('backend')

        if backend is None:
            print("Missing 'backend' in %s"%connection_name)

        print(backend)

clients = db_config.get('clients')
if clients:
    print(clients)

    for client_name in clients:
        print(client_name)
        connections = clients[client_name]
        print(connections)
