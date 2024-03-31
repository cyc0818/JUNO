#include "RandomSvc.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/Task.h"
#include "CLHEP/Random/Random.h"
#include "RandomSeedRecorder.h"
#include "TRandom.h"

DECLARE_SERVICE(RandomSvc);

RandomSvc::RandomSvc(const std::string& name)
    : SvcBase(name)
{
    declProp("Seed", m_init_seed);
    declProp("SeedStatusInputFile", m_ss_input="");
    declProp("SeedStatusInputVector", m_seed_status_in);
}

RandomSvc::~RandomSvc()
{

}

bool
RandomSvc::initialize()
{
    LogDebug << "Initialize the SEED="
            << m_init_seed
            << std::endl;
    setSeed(m_init_seed);

    // if the SeedStatusInputVector is not null, that means we need to
    // restore the status.
    if (m_seed_status_in.size()) {
        LogDebug << "Restore the Engine status" << std::endl;
        CLHEP::HepRandom::getTheEngine()->get(m_seed_status_in);
    }

    // == register the incident ==
    // Task* par = getScope();
    // IIncidentHandler* bi = new RandomSeedRecorder(par);
    // if ( par->isTop() ) {
    //     bi->regist("BeginEvent");
    // } else {
    //     bi->regist(par->scope() + par->objName() + ":BeginEvent");
    // }
    // m_icdts.push_back(bi);

    Task* par = dynamic_cast<Task*>(getParent());
    IIncidentHandler* bi = new RandomSeedRecorder(par);
    bi->regist("BeginEvent");
    m_icdts.push_back(bi);


    return true;
}

bool
RandomSvc::finalize()
{
    for (std::vector<IIncidentHandler*>::iterator it = m_icdts.begin();
            it != m_icdts.end(); ++it) {
        delete (*it);
        (*it) = 0;
    }

    LogDebug << "Finalize the SEED="
             << getSeed()
             << std::endl;
    return true;
}

long
RandomSvc::getSeed()
{
    // before return the seed, we can show the current status 
    if (logLevel() <= 2) {
        CLHEP::HepRandom::showEngineStatus();
        // get the vector of seed status
        std::vector<unsigned long> result = CLHEP::HepRandom::getTheEngine()
                                                -> put();
        std::cout << "Random Engine Seed Status: ";
        for (size_t i = 0; i < result.size(); ++i) {
            std::cout << result[i] << ", ";
        }
        std::cout << std::endl;
    }

    return CLHEP::HepRandom::getTheSeed();
}

void
RandomSvc::setSeed(long seed) 
{
    CLHEP::HepRandom::setTheSeed(seed);
    // Add ROOT support
    gRandom->SetSeed(seed);
}

double 
RandomSvc::random() {
    return CLHEP::HepRandom::getTheGenerator()->flat();
}

