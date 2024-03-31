#include <boost/python.hpp>

void export_CondDB_GlobalTag();
void export_CondDB_Tag();
void export_CondDB_IOV();
void export_CondDB_Payload();
void export_CondDB_IRepo();
void export_CondDB_DBRepo();
void export_CondDB_LocalFSRepo();
void export_CondDB_ICondDBSvc();
void export_CondDB_CondDBSvc();

void export_CondDB_IConditionObject();
void export_CondDB_ICnvFromPayload();

void export_CondDB_Testing_CD_LS_MatProp();


BOOST_PYTHON_MODULE(libCondDB)
{
    export_CondDB_GlobalTag();
    export_CondDB_Tag();
    export_CondDB_IOV();
    export_CondDB_Payload();

    export_CondDB_IRepo();
    export_CondDB_DBRepo();
    export_CondDB_LocalFSRepo();

    export_CondDB_ICondDBSvc();
    export_CondDB_CondDBSvc();

    export_CondDB_IConditionObject();
    export_CondDB_ICnvFromPayload();

    export_CondDB_Testing_CD_LS_MatProp();

}
