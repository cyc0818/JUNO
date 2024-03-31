#include <boost/python.hpp>
#include <MCParamsFileSvc.hh>

#include <fstream>
#include <sstream>

#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"

#include "utils.hh"

namespace fs = boost::filesystem;

DECLARE_SERVICE(MCParamsFileSvc);

MCParamsFileSvc::MCParamsFileSvc(const std::string& name)
    : SvcBase(name)
{

}

MCParamsFileSvc::~MCParamsFileSvc()
{


}

bool
MCParamsFileSvc::initialize()
{
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
    return get_implv1(param, props);
}

bool
MCParamsFileSvc::Get(const std::string& param, vec_s2d& props)
{
    return get_implv1(param, props);
}

bool
MCParamsFileSvc::Get(const std::string& param, map_s2d& props)
{
    // convert tuple to map
    bool st;
    vec_s2d vec_props;
    st = get_implv1(param, vec_props);
    for (vec_s2d::iterator it = vec_props.begin();
            it != vec_props.end(); ++it) {
        props[it->get<0>()] = it->get<1>();
    }

    return st;
}
