#ifndef GtNuSolTool_h
#define GtNuSolTool_h

/*
 * Interface for Solar neutrino generator
 */

#include "SniperKernel/ToolBase.h"
#include "GenTools/IGenTool.h"

#include "HepMC/SimpleVector.h"
#include "HepMC/GenParticle.h"

class TTree;

namespace NuSol {
    class Imutate;
}

class GtNuSolTool: public ToolBase,
                 public IGenTool
{
    public:
        GtNuSolTool(const std::string& name);
        virtual ~GtNuSolTool();

        bool configure();
        bool mutate(HepMC::GenEvent& event);

    private:
        std::string m_inputMode;
	bool m_saveVertex;
	std::string m_saveRootFileName;

	std::string m_inputFile;
	std::string m_treeName;
	int m_startIndex;

	int m_seed;
	std::string m_neutrinoType;
	NuSol::Imutate *m_eventSource; // definition see NuSolGen/NuSol.h
};
#include "SolarNeutrinoSpectrum.hh"
namespace NuSol {
    class Imutate {
	public:
	    virtual bool mutate(double &electron_kinetic_energy) = 0;
    };
    class fromRootFile : public Imutate {
	public:
	    virtual bool mutate(double &electron_kinetic_energy);
	    bool SetInputTTree(std::string &inputFile,std::string &treeName);
	    bool SetStartIndex(int startIndex);
	private:
	    int m_index;
	    int m_maxIndex;
	    double m_electron_kinetic_energy;
	    TTree *m_tree;
    };
    class generateRealTime : public Imutate {
	public:
	    bool Initialize(std::string &neutrinoType);
	    virtual bool mutate(double &electron_kinetic_energy);
	private:
	    SolarNeutrinoSpectrum m_solargen;
    };
}


#endif
