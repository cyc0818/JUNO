/** HepEvt2HepMC - parse HEPEvt files into HepMC::GenEvents 
 *
 * bv@bnl.gov 2007/12/31.
 */
/*
 * Stolen from Nuwa.
 */

#ifndef HEPEVT2HEPMC_H
#define HEPEVT2HEPMC_H

namespace HepMC {
    class GenEvent;
}

#include <string>
#include <list>
#include <cstdio>

class HepEvt2HepMC {
public:
    HepEvt2HepMC(const std::string& filename);
    ~HepEvt2HepMC();
    
    /// Remove next event from the cache and return it.
    bool generate(HepMC::GenEvent& event);

    /// Fill up the cache using the source description.  This can be a
    /// file name or an executable command followed by a "|" symbol.
    bool fill(HepMC::GenEvent& event);

    /// Number of primary vertices left in the cache
    // int cacheSize() { return m_events.size(); }

private:
    // std::list<HepMC::GenEvent*> m_events;
    int m_eventCount;           // total over all fill()s

    std::string m_filename;
    FILE* m_fp;
};

#endif  // HEPEVT2HEPMC_H
