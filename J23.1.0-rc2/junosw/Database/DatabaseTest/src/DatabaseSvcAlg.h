#ifndef DATABASESVCALG_H
#define DATABASESVCALG_H

#include "SniperKernel/AlgBase.h"

class DatabaseSvcAlg: public AlgBase
{
 public:
    DatabaseSvcAlg(const std::string& name);
    virtual ~DatabaseSvcAlg();

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();
    
};

#endif
