#ifndef GRDMSelector_hh
#define GRDMSelector_hh

#include <vector>
#include <fstream>

struct GRDMRecord {
    int pdgcode;
    double mass; // runtime query from geant4
    double ratio;
    int Amin;
    int Amax;
    int Zmin;
    int Zmax;

    void Dump() const;
};

class GRDMSelector {
public:
    GRDMSelector(std::ifstream&);

    bool is_available();

    // get one of the record
    const GRDMRecord* get_randomly();
    
    const std::vector<GRDMRecord>& records();
    const std::vector<double>& cdfs();

private:
    std::vector<GRDMRecord> m_records;
    std::vector<double> m_cdfs;
};

#endif
