#include <boost/python.hpp>
#include <MCParamsCrestSvc.hh>

#include <fstream>
#include <sstream>

#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"

#include "Rest/cURL.h"
#include "Rest/object.h"

// base64 decoder in ROOT
#include "TBase64.h"

#include "utils.hh"

DECLARE_SERVICE(MCParamsCrestSvc);


/*
 * To get payload from CrestDB, we need to define object model.
 */

struct Payload: Rest::object {
    std::string hash;
    std::string version;
    std::string object_type;
    std::string data;
    std::string streamer_info;
    std::string insertion_time;

    Payload() {
        decl("hash", hash);
        decl("version", version);
        decl("objectType", object_type);
        decl("data", data);
        decl("streamerInfo", streamer_info);
        decl("insertionTime", insertion_time);
    }
};



MCParamsCrestSvc::MCParamsCrestSvc(const std::string& name)
    : SvcBase(name)
{
    declProp("URI", m_uri);
}

MCParamsCrestSvc::~MCParamsCrestSvc()
{


}

bool
MCParamsCrestSvc::initialize()
{
    // initialize all MC parameters
    Rest::cURL curlobj;

    std::string uri;

    uri = m_uri + ""; // 
    if (!curlobj.request(uri)) {
        LogError << "access " << uri << " failed." << std::endl;
        return false;
    }

    cJSON* json = cJSON_Parse(curlobj.result().c_str());
    bool is_array = false;

    if (json->type==cJSON_Array) {
        std::cout << "It's array!" << std::endl;
        is_array = true;
    } else {
        std::cout << "It's object!" << std::endl;
        
    }

    for (cJSON* elem = (is_array) ? json->child: json; 
         elem != NULL; elem = elem->next) {

        Payload p;
        p.from_json(elem);
        p.show();

        // decode payload
        TString result = TBase64::Decode(p.data.c_str());
        std::cout << result << std::endl;

        // according to the streamerInfo, put data into container
        if (p.streamer_info == "vec_d2d") {
            // parse the result
            IMCParamsSvc::vec_d2d vec;

            bool st = parse_implv1(result.Data(), vec);

            if (!st) {
                LogError << "Failed to parse payload " << p.object_type << std::endl;
                return false;
            }

            m_vec_d2d[p.object_type] = vec;
            LogInfo << "Add " << p.object_type << std::endl;

        } else if (p.streamer_info == "vec_s2d") {
            // parse the result
            IMCParamsSvc::vec_s2d vec;

            bool st = parse_implv1(result.Data(), vec);

            if (!st) {
                LogError << "Failed to parse payload " << p.object_type << std::endl;
                return false;
            }

            m_vec_s2d[p.object_type] = vec;
            LogInfo << "Add " << p.object_type << std::endl;

        } else if (p.streamer_info == "map_s2d") {

            IMCParamsSvc::vec_s2d vec;
            bool st = parse_implv1(result.Data(), vec);

            if (!st) {
                LogError << "Failed to parse payload " << p.object_type << std::endl;
                return false;
            }

            // assume vec_s2d is initialized now
            IMCParamsSvc::map_s2d props;
            for (IMCParamsSvc::vec_s2d::iterator it = vec.begin();
                 it != vec.end(); ++it) {
                props[it->get<0>()] = it->get<1>();
            }

            m_map_s2d[p.object_type] = props;


        } else {
            LogError << "Unknow streamer_info: " << p.streamer_info << std::endl;
        }
    }        


    return true;
}

bool
MCParamsCrestSvc::finalize()
{
    return true;
}



bool
MCParamsCrestSvc::Get(const std::string& param, vec_d2d& props)
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
MCParamsCrestSvc::Get(const std::string& param, vec_s2d& props)
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
MCParamsCrestSvc::Get(const std::string& param, map_s2d& props)
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
