#include <boost/python.hpp>
#include <fstream>
#include <sstream>

#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"
#include "Parameter/DetParamsFileSvc.hh"
#include "Parameter/utils.hh"

namespace fs = boost::filesystem;

DECLARE_SERVICE(DetParamsFileSvc);

DetParamsFileSvc::DetParamsFileSvc(const std::string &name)
    : SvcBase(name)
{
    declProp("ReplaceParam", m_Param2value);
}

DetParamsFileSvc::~DetParamsFileSvc()
{
}

bool DetParamsFileSvc::initialize()
{
    
    return true;
}

bool DetParamsFileSvc::finalize()
{
    return true;
}

bool DetParamsFileSvc::Get(const std::string &param, vec_d2d &props)
{
    const std::string path = GetPath(param);
    return get_implv1(path, props);
}

bool DetParamsFileSvc::Get(const std::string &param, vec_s2d &props)
{
    const std::string path = GetPath(param);
    return get_implv1(path, props);
}

bool DetParamsFileSvc::Get(const std::string &param, map_s2d &props)
{
    // convert tuple to map
    bool st;
    vec_s2d vec_props;

    const std::string path = GetPath(param);
    st = get_implv1(path, vec_props);
    for (vec_s2d::iterator it = vec_props.begin();
         it != vec_props.end(); ++it)
    {
        std::string property = param;
        property = property + "." + it->get<0>();
        //  check the parameters which need to be replaced
        if (m_Param2value.count(property))
        {
            std::string value = m_Param2value[property];
            double num_value = g4string2value(value);
            props[it->get<0>()] = num_value;
            LogInfo << "Note you will use your own parameter:" << property << "  value is " << num_value << std::endl;
        }
        else
        {
            props[it->get<0>()] = it->get<1>();
        }
    }

    return st;
}

const std::string DetParamsFileSvc::GetPath(const std::string &param)
{

    namespace fs = boost::filesystem;
    // check the parameters which need to be replaced
    if (m_Param2value.count(param))
    {
        std::string path = m_Param2value[param];
        fs::path file(path);
        if (!fs::exists(file))
        {
            LogError << "Path " << file.string() << " does not exist." << std::endl;
            return 0;
        }
        const std::string path_c = path;
        LogInfo << "Note you will load your own parameters from the file:" << path_c << std::endl;
        return path_c;
    }

    std::string base;
    if (getenv("JUNOTOP"))
    {
        base = getenv("JUNOTOP");
        base += "/data/Detector/Parameter";
    }
    if (getenv("WORKTOP"))
    {
        std::string worktop = getenv("WORKTOP");
        worktop += "/data/Detector/Parameter";
        fs::path worktop_path(worktop);
        if (fs::exists(worktop_path))
        {
            base = worktop;
        }
    }
    if (getenv("DETPARAMSROOT"))
    {
        base = getenv("DETPARAMSROOT");
    }

    fs::path fullp(base);
    if (!fs::exists(fullp))
    {
        LogError << "Path: " << fullp.string() << " does not exists. " << std::endl;
        LogError << "Check $DETPARAMSROOT or $JUNOTOP/data/Detector/Parameter or $WORKTOP/data/Detector/Parameter " << std::endl;
        exit(0);
        return 0;
    }
    else
    {
        LogInfo << "Detector parameters will be loaded from: " << base << std::endl;
    }

    // magic here, convert it to path
    // + Material
    //   + LS
    //     + RINDEX - last one should be a file
    boost::char_separator<char> sep(".");
    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
    tokenizer tok(param, sep);
    for (tokenizer::iterator it = tok.begin(); it != tok.end(); ++it)
    {
        std::string s = *it;
        fs::path sub(s);
        fullp /= sub;
    }

    if (!fs::exists(fullp))
    {
        LogError << "Path " << fullp.string() << " does not exist." << std::endl;
        return 0;
    }

    const std::string path = fullp.string();
    return path;
}

double DetParamsFileSvc::g4string2value(std::string value)
{
    double num = 0;
    std::string::size_type position;
    std::string::size_type position_1;
    std::string::size_type position_2;
    position_1 = value.find_first_of("*");
    position_2 = value.find_first_of("/");

    if ((position_1 == std::string::npos) && (position_2 == std::string::npos))
    {
        num = std::stod(value);
        std::cout << "warning: replace value has no unit! value == " << num << std::endl;
        return num;
    }

    if (position_1 <= position_2)
    {
        position = position_1;
    }
    else
    {
        position = position_2;
    }
    std::string number_string = value.substr(0, position);
    std::string unit_string = value.substr(position);
    num = std::stod(number_string);
    with_units(num, unit_string);
    return num;
}
