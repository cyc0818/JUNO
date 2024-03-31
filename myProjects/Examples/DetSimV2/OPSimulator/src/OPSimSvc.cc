#include <boost/python.hpp>
#include "OPSimSvc.h"

#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"

#include "G4OPSimulator.h"

DECLARE_SERVICE(OPSimSvc);

OPSimSvc::OPSimSvc(const std::string& name)
    : SvcBase(name)
    , m_opsimulator(nullptr)
{
    declProp("SimulatorType", m_simulator_type="G4OPSimulator");
}

OPSimSvc::~OPSimSvc()
{

}

bool OPSimSvc::initialize()
{
    if (m_simulator_type=="G4OPSimulator") {
        m_opsimulator = new opsimulator::G4OpSimulator();
    } else {
        LogError << "Unsupportted OP simulator: " << m_simulator_type << std::endl;
        return false;
    }

    return true;
}

bool OPSimSvc::finalize()
{
    return true;
}

opsimulator::IOPSimulator* OPSimSvc::get_opsimulator()
{
    return m_opsimulator;
}
