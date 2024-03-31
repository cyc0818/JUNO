#ifndef DATABASESPMSALG_H
#define DATABASESPMSALG_H

#include "SniperKernel/AlgBase.h"

class DatabaseSpmsAlg: public AlgBase
{
 public:
    DatabaseSpmsAlg(const std::string& name);
    virtual ~DatabaseSpmsAlg();

    virtual bool initialize();
    virtual bool execute();
    virtual bool finalize();

 private:
    std::string id;
    std::string Mode;
    
};

#endif
