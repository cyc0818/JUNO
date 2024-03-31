/**
 * \class Radiation
 *
 * \brief Types of radiation
 *
 * Holds classes describing specific types of radiation (alpha, beta
 * gamma) between specific states.
 *
 * bv@bnl.gov Tue Feb  3 14:57:27 2009
 *
 */



#ifndef RADIATION_H
#define RADIATION_H

//#include "GaudiKernel/RndmGenerators.h"
#include <string>
#include <iostream>
#include "RandomSvc/IRandomSvc.h"
namespace GenDecay {

enum RadiationType {
    Unknown = 0,
    Alpha, BetaMinus, BetaPlus, Gamma, EleCapture,
};

class Radiation {
protected:

    double m_energy;

public:

    Radiation(double characteristic_energy);
    virtual ~Radiation();

    virtual double kineticEnergy() const;


    virtual std::string asString() const = 0;

    virtual RadiationType type() const  = 0;
    virtual int pid() const = 0;
    virtual double mass() const = 0;
    virtual std::string typeString() const  = 0;

};


class AlphaRadiation : public Radiation 
{
    int m_parentA;

public:
    // For Alpha, energy is kinetic energy
    AlphaRadiation(double energy, int parentA);
    virtual ~AlphaRadiation();

    std::string asString() const;

    double kineticEnergy() const;
    int pid() const;
    double mass() const;

    RadiationType type() const { return Alpha; }
    std::string typeString() const { return "Alpha"; }
};

class BetaRadiation : public Radiation 
{
    int m_parentZ;
    int m_daughterZ;
    int m_betaSign;
    double m_endpoint;

    double m_norm, m_maximum;
//    mutable Rndm::Numbers m_rand; // not const correct....

    void normalize();
    
    IRandomSvc* m_rs;

public:
    // For beta, energy is endpoint energy, not corrected for electron loss (beta+)
    BetaRadiation(double energy, int parentZ);
    virtual ~BetaRadiation();

    RadiationType type() const { return (m_betaSign < 0 ? BetaMinus : BetaPlus); }
    std::string typeString() const { return (m_betaSign < 0 ? "BetaMinus" : "BetaPlus"); }

    std::string asString() const;

    int sign() const { return m_betaSign; }

    double kineticEnergy() const;
    int pid() const;
    double mass() const;

    double dnde(double kineticEnergy) const;
    double dnde_noff(double kineticEnergy) const;
    double fermi_function(double kineticEnergy) const;
};

class GammaRadiation : public Radiation 
{
public:
    GammaRadiation(double energy);
    virtual ~GammaRadiation();

    int pid() const;
    double mass() const;

    std::string asString() const;
    RadiationType type() const { return Gamma; }
    std::string typeString() const { return "Gamma"; }
};

class ElectronCapture: public Radiation
{
public:
    ElectronCapture(double characteristic_energy);
    virtual ~ElectronCapture();

    std::string asString() const;
    int pid() const;
    double mass() const;
    RadiationType type() const { return EleCapture; }
    std::string typeString() const { return "EleCapture"; }
};

std::ostream& operator<<(std::ostream& o, const Radiation& r);

} // namespace GenDecay



#endif  // RADIATION_H
