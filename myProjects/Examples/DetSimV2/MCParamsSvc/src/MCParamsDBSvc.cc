#include <boost/python.hpp>
#include <MCParamsDBSvc.hh>

#include <fstream>
#include <sstream>

#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include "DatabaseSvc/DatabaseSvc.h"
#include "DatabaseSvc/DBIResultPtr.h"

// base64 decoder in ROOT
#include "TBase64.h"

#include "utils.hh"

DECLARE_SERVICE(MCParamsDBSvc);


/*
 * To get payload from CrestDB, we need to define object model.
 */



MCParamsDBSvc::MCParamsDBSvc(const std::string& name)
    : SvcBase(name)
{
    declProp("URI", m_uri);
}

MCParamsDBSvc::~MCParamsDBSvc()
{


}

bool
MCParamsDBSvc::initialize()
{
    // access database service
    SniperPtr<DatabaseSvc> dbsvc(getParent(), "DatabaseSvc");
    if (dbsvc.invalid()) {
        LogError << "Can't access DatabaseSvc." << std::endl;
        return false;
    }
    const std::string query_str = "select * from offline_db.Material_LS"
        " where id=3";
    DBIRequest request(query_str, query);

    dbsvc->Session(request);

    DBIResult& resultset = dbsvc->FetchResult();
    LogInfo << "result set: " << resultset.GetResult() << std::endl;
    sql::ResultSet* rs = resultset.GetResult();

    if (!rs) {
        LogInfo << "failed to execute sql statement '"
                << query_str << "'" << std::endl;
        return false;
    }

    LogInfo << "size of result set: " << rs->rowsCount() << std::endl;

    // assume only one result
    rs->next();

    // LogInfo << "ABSLENGTH: " << rs->getString("ABSLENGTH") << std::endl;

    struct P {
        const char* name;
        const char* streamer;
    } payloads[] = {
    {"Material.LS.RINDEX", "vec_d2d"},
    {"Material.LS.ABSLENGTH", "vec_d2d"},
    {"Material.LS.ABSLENGTHwithUnits", "vec_d2d"},
    {"Material.LS.FASTCOMPONENT", "vec_d2d"},
    {"Material.LS.REEMISSIONPROB", "vec_d2d"},
    {"Material.LS.RAYLEIGH", "vec_d2d"},
    {"Material.LS.SCINTILLATIONYIELD", "vec_d2d"},
    {"Material.LS.RESOLUTIONSCALE", "vec_d2d"},
    {"Material.LS.GammaFASTTIMECONSTANT", "vec_d2d"},
    {"Material.LS.GammaSLOWTIMECONSTANT", "vec_d2d"},
    {"Material.LS.GammaYIELDRATIO", "vec_d2d"},
    {"Material.LS.AlphaFASTTIMECONSTANT", "vec_d2d"},
    {"Material.LS.AlphaSLOWTIMECONSTANT", "vec_d2d"},
    {"Material.LS.AlphaYIELDRATIO", "vec_d2d"},
    {"Material.LS.NeutronFASTTIMECONSTANT", "vec_d2d"},
    {"Material.LS.NeutronSLOWTIMECONSTANT", "vec_d2d"},
    {"Material.LS.NeutronYIELDRATIO", "vec_d2d"},
    {"Material.LS.scale", "vec_s2d"},
    {"Material.LS.scale", "map_s2d"},
    {NULL, NULL}
    };

    for (struct P* p = payloads; p->name != NULL; ++p) {
        // don't need to copy the string again.
        const char* bn = p->name + 12; // the magic number 12 is "Material.LS"
        LogInfo << bn << std::endl;

        TString result = TBase64::Decode(rs->getString(bn).c_str());

        if (strcmp(p->streamer, "vec_d2d") == 0) {
            IMCParamsSvc::vec_d2d vec;

            bool st = parse_implv1(result.Data(), vec);

            if (!st) {
                LogError << "Failed to parse payload " << bn << std::endl;
                return false;
            }

            m_vec_d2d[p->name] = vec;
            LogInfo << "Add " << p->name << std::endl;

        } else if (strcmp(p->streamer, "vec_s2d") == 0) {
            IMCParamsSvc::vec_s2d vec;

            bool st = parse_implv1(result.Data(), vec);

            if (!st) {
                LogError << "Failed to parse payload " << bn << std::endl;
                return false;
            }

            m_vec_s2d[p->name] = vec;
            LogInfo << "Add " << p->name << std::endl;

        } else if (strcmp(p->streamer, "map_s2d")==0) {
            IMCParamsSvc::vec_s2d vec;
            bool st = parse_implv1(result.Data(), vec);

            if (!st) {
                LogError << "Failed to parse payload " << bn << std::endl;
                return false;
            }

            // assume vec_s2d is initialized now
            IMCParamsSvc::map_s2d props;
            for (IMCParamsSvc::vec_s2d::iterator it = vec.begin();
                 it != vec.end(); ++it) {
                props[it->get<0>()] = it->get<1>();
            }

            m_map_s2d[p->name] = props;
            LogInfo << "Add " << p->name << std::endl;
        } else {
            LogError << "Unknow streamer_info: " << p->streamer << std::endl;
            return false;
        }

    }

    // DBIResultPtr dbiptr(getScope(), "DatabaseSvc", request);
    // dbiptr.Session();

    // int size_rows = dbiptr.GetMaxRowcount();
    // std::cout << "Size of rows: " << size_rows << std::endl;
    
    // // loading parameters from database
    // // - Material.LS.XYZ
    // for (int i = 0; i < size_rows; ++i) {
    //     dbiptr.GetResByRowNum(i);
    //     std::cout << " Row " << i << " "
    //               << dbiptr.GetString("ABSLENGTH")
    //               << std::endl;
    // }


    return true;
}

bool
MCParamsDBSvc::finalize()
{
    return true;
}



bool
MCParamsDBSvc::Get(const std::string& param, vec_d2d& props)
{
    bool st = false;

    std::map<std::string, IMCParamsSvc::vec_d2d>::iterator it = m_vec_d2d.find(param);
    if (it != m_vec_d2d.end()) {
        st = true;
        props = it->second;
    }

    return st;
}

bool
MCParamsDBSvc::Get(const std::string& param, vec_s2d& props)
{
    bool st = false;

    std::map<std::string, IMCParamsSvc::vec_s2d>::iterator it = m_vec_s2d.find(param);
    if (it != m_vec_s2d.end()) {
        st = true;
        props = it->second;
    }

    return st;
}

bool
MCParamsDBSvc::Get(const std::string& param, map_s2d& props)
{
    // convert tuple to map
    bool st = false;

    std::map<std::string, IMCParamsSvc::map_s2d>::iterator it = m_map_s2d.find(param);
    if (it != m_map_s2d.end()) {
        st = true;
        props = it->second;
    }

    return st;
}
