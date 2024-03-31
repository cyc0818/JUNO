
#include "GRDMSelector.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4NucleiProperties.hh"
#include <iostream>
#include <sstream>

void GRDMRecord::Dump() const {
    std::cout << " * DUMP GRDMRecord"
              << "pdg: " << pdgcode << " "
              << "mass: " << mass << " "
              << "ratio: " << ratio << " "
              << "A: [" << Amin << "," << Amax << "] "
              << "Z: [" << Zmin << "," << Zmax << "] "
              << std::endl;            

}

GRDMSelector::GRDMSelector(std::ifstream& input) {
    double cdf = 0.0;
    // parse the file
    std::string line;
    while (std::getline(input, line)) {
        int _pdgcode;
        double _mass;
        double _ratio;
        int _Amin;
        int _Amax;
        int _Zmin;
        int _Zmax;

        std::stringstream ss(line);
        ss >> _pdgcode
           >> _ratio
           >> _Amin
           >> _Amax
           >> _Zmin
           >> _Zmax;

        if (ss.fail()) {
            continue;
        }
        
        if (_pdgcode >= 1000000000) {
            int encoding = _pdgcode;
            encoding -= 1000000000;
            int Z = encoding/10000;
            encoding -= 10000*Z;
            int A = encoding/10;
            _mass = G4NucleiProperties::GetNuclearMass(A, Z);

            GRDMRecord record;
            record.pdgcode = _pdgcode;
            record.mass = _mass;
            record.ratio = _ratio;
            record.Amin = _Amin;
            record.Amax = _Amax;
            record.Zmin = _Zmin;
            record.Zmax = _Zmax;

            m_records.push_back(record);

            cdf += _ratio;

            m_cdfs.push_back(cdf);
        }
    }

    // normalized
    std::cout << "GRDMSelector details: " << std::endl;
    for (int i = 0; i < m_cdfs.size(); ++i) {
        double r = m_cdfs[i];
        m_cdfs[i] = r/cdf;
        std::cout << " * [" << i << "] "
                  << "pdg: " << m_records[i].pdgcode << " "
                  << "mass: " << m_records[i].mass << " "
                  << "ratio: " << m_records[i].ratio << " "
                  << "cdf: " << m_cdfs[i] << " "
                  << "A: [" << m_records[i].Amin << "," << m_records[i].Amax << "] "
                  << "Z: [" << m_records[i].Zmin << "," << m_records[i].Zmax << "] "
                  << std::endl;            
        m_records[i].Dump();
    }
}

bool GRDMSelector::is_available() {
    return m_records.size()>0;
}

const GRDMRecord* GRDMSelector::get_randomly() {
    if (not is_available()) {
        return nullptr;
    }

    size_t sample_idx = 0;
    double r = CLHEP::RandFlat::shoot();
    for (size_t i = 0; i < m_cdfs.size(); ++i) {
        // select the sample key
        if (r < m_cdfs[i]) {
            // HERE we find it
            sample_idx = i;
            break; // found it, so skip the next loop
        }
    }
    return &m_records[sample_idx];
}

const std::vector<GRDMRecord>& GRDMSelector::records() {
    return m_records;
}

const std::vector<double>& GRDMSelector::cdfs() {
    return m_cdfs;
}
