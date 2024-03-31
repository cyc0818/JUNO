#include <boost/python.hpp>
#include <MCParamsFileSvc.hh>
#include <ParaDB/IParaDBSvc.hh>
#include <memory>
#include <fstream>
#include <sstream>

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SharedElemFactory.h"

#include "utils.hh"

namespace fs = boost::filesystem;

DECLARE_SERVICE(MCParamsFileSvc);
DECLARE_CAN_BE_SHARED(MCParamsFileSvc);

MCParamsFileSvc::MCParamsFileSvc(const std::string& name)
    : SvcBase(name), m_paradb_svc(nullptr)
{
  declProp("ReplaceParam",m_Param2value);
  declProp("UseParaDB", m_paradb_enabled=true);
}

MCParamsFileSvc::~MCParamsFileSvc()
{


}

bool
MCParamsFileSvc::initialize()
{
    if(getenv("JUNOTOP")){
        m_base = getenv("JUNOTOP");
        LogInfo << "The offical MC Param data directory is under JUNOTOP: " << m_base << std::endl;
    } else {
        LogError << "Failed to get JUNOTOP. " << std::endl;
        return false;
    }

    if(getenv("WORKTOP")){
        m_worktop_base = getenv("WORKTOP");
        LogInfo << "The user defined MC Param data directory could be under WORKTOP: " 
                << m_worktop_base << std::endl;
    }

    if (m_paradb_enabled) {
        SniperPtr<ParaDB::IParaDBSvc> paradb(getParent(), "ParaDBSvc");
        if (paradb.invalid()) {
            LogError << "Failed to get ParaDBSvc. " << std::endl;
            return false;
        }

        m_paradb_svc = paradb.data();
    }

    if (m_paradb_svc) {
        LogInfo << "MCParamsFileSvc is initalized with ParaDB. " << std::endl;
        LogInfo << "The default base path is " << m_base << std::endl;
    }

    return true;
}

bool
MCParamsFileSvc::finalize()
{
    return true;
}



bool
MCParamsFileSvc::Get(const std::string& param, vec_d2d& props)
{  
     const std::string path = GetPath( param );
    return get_implv1(path, props); 
}

bool
MCParamsFileSvc::Get(const std::string& param, vec_s2d& props)
{
    const std::string path = GetPath( param );
    return get_implv1(path, props);
}

bool
MCParamsFileSvc::Get(const std::string& param, map_s2d& props)
{
    // convert tuple to map
    bool st;
    vec_s2d vec_props;

    const std::string path = GetPath( param );
    st = get_implv1(path, vec_props);
    for (vec_s2d::iterator it = vec_props.begin();
            it != vec_props.end(); ++it) {
        std::string  property = param;
        property = property + "."+it->get<0>();
      //  check the parameters which need to be replaced
        if( m_Param2value.count(property)){
           std::string value = m_Param2value[property];
           double num_value = g4string2value(value);
           props[it->get<0>()] = num_value; 
           LogInfo<<"Note you will use your own parameter:"<<property<<"  value is "<<num_value<<std::endl;  
        }
        else{
          props[it->get<0>()] = it->get<1>();
        }
    }

    return st;
}

 
const std::string MCParamsFileSvc::GetPath(const std::string &param ) {
    // priorities:
    // 1. specified at command line / properties
    // 2. from paradb (official path)
    // 3. under WORKTOP/JUNOTOP (for developers only)

    // =======================================================================
    // COMMAND LINE / PROPERTIES
    // =======================================================================
    namespace fs = boost::filesystem;
    //  check the parameters which need to be replaced 
    if(m_Param2value.count(param)){
        std::string path = m_Param2value[param];
        fs::path file(path);
        if (!fs::exists(file)) {
            LogError << "Path " << file.string() << " does not exist." << std::endl;
            return 0;
        } 
        const std::string path_c = path;
        LogInfo<<"Note you will use your own parameters from the file:"<<path_c<<std::endl;
        return path_c;
    }

    // =======================================================================
    // ParaDB
    // =======================================================================
    // As the ParaDB only returns relative paths, the JUNOTOP is 
    // used by default.

    if (m_paradb_svc and is_available_in_paradb(param)) {
        std::string result;

        // a special case: in the previous implemantation, the version is 
        // also included in the property name, so we need to remove the 
        // version. 
        // such as 'Material.LS.ABSLENGTH_v1' will become
        //         'Material.LS.ABSLENGTH'
        auto prop_name = get_paradb_property_name(param);

        if (not m_paradb_svc->hasParameter(prop_name)) {
            LogError << "Cannot find parameter " << prop_name << std::endl;
            return result;
        }
        auto prop = m_paradb_svc->getParameter(prop_name);
        
        fs::path file(m_base + prop.path()); // note: prop.path() starts with '/'
        if (!fs::exists(file)) {
            LogError << "Path " << file.string() << " does not exist." << std::endl;
            return result;
        }

        result = m_base + prop.path();
        return result;
        
    }


    // =======================================================================
    // WORKTOP or JUNOTOP
    // =======================================================================


    std::string base;
    if(m_base.size()){
        base = m_base + "/data/Simulation/DetSim";
    }
    if (m_worktop_base.size()) {
        std::string worktop = m_worktop_base;
        worktop += "/data/Simulation/DetSim";
        fs::path worktop_path(worktop);
        if (fs::exists(worktop_path)) {
            base = worktop;
        }

    }
    if(getenv("MCPARAMSROOT")){
        base = getenv("MCPARAMSROOT");
    }

    fs::path fullp(base);
    if (!fs::exists(fullp)) {
        LogError << "Param: " << param <<  " Path: " << fullp.string() << " does not exists. " << std::endl;
        LogError << "Check $MCPARAMSROOT or $JUNOTOP/data/Simulation/DetSim or $WORKTOP/data/Simulation/DetSim " << std::endl;
        exit(0);
        return 0;
    }
    else{
        LogInfo<<"Optical parameters will be used from: "<<base<<std::endl;
    }

    // magic here, convert it to path
    // + Material
    //   + LS
    //     + RINDEX - last one should be a file
    boost::char_separator<char> sep(".");
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    tokenizer tok(param, sep);
    for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
        std::string s = *it;
        fs::path sub(s);
        fullp /= sub;
    }
          
    if (!fs::exists(fullp)) {
        LogError << "Path " << fullp.string() << " does not exist." << std::endl;
        return 0;
    }
        
    const std::string path = fullp.string();
    return path;
}

double MCParamsFileSvc::g4string2value(std::string value){
    double num = 0 ;
    std::string::size_type position;
    std::string::size_type position_1;
    std::string::size_type position_2;
    position_1 = value.find_first_of("*");
    position_2 = value.find_first_of("/");
 
    if ( (position_1 == std::string::npos) && (position_2 == std::string::npos) ){
      num = std::stod(value);
      std::cout<<"warning: replace value has no unit! value == "<<num<<std::endl;
      return num ;
    
    }
     
    if ( position_1 <= position_2){
      position = position_1;
    }
    else{
      position = position_2;
    }
   std::string number_string = value.substr(0,position);
   std::string unit_string = value.substr(position);
   num = std::stod(number_string);
   with_units(num,unit_string);
   return num;
}

#include <regex>

std::string MCParamsFileSvc::get_paradb_property_name(const std::string& param) {
    const static std::regex version_suffix("_v\\d+$");

    return std::regex_replace(param, version_suffix, "");;
}

bool MCParamsFileSvc::is_available_in_paradb(const std::string& param) {
    const static std::string white_lists[] = {
        "Material"
    };

    for (auto prefix_name: white_lists) {
        if (param.find(prefix_name, 0) == 0) {
            return true;
        }
    }

    return false;
}
