#include "HepEvt2HepMC.h"
#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cerrno>

static FILE* open_file(const std::string& filename);
static void close_file(FILE* fp, const std::string& filename);

HepEvt2HepMC::HepEvt2HepMC(const std::string& filename)
    : m_eventCount(0), m_filename(filename), m_fp(0)
{
    // open the file or execute the generator when HepEvt2HepMC is created. 
    if (!m_fp) {
        // reset errno first
        errno = 0;
        m_fp = open_file(m_filename);
        if ((!m_fp) or errno) {
            std::string msg = "Failed to open HEPEvt data source: \"";
            msg += m_filename;
            msg += "\"";
            std::cerr << msg << std::endl;
            m_fp = 0;
        }
    }

}

HepEvt2HepMC::~HepEvt2HepMC()
{
    std::cerr << "HepEvt2HepMC Dtor called begin" << std::endl;
    close_file(m_fp, m_filename);
    std::cerr << "HepEvt2HepMC Dtor called end" << std::endl;
}


bool HepEvt2HepMC::generate(HepMC::GenEvent& event)
{
    if (not fill(event)) {
        return false;
    }
    return true;
}


#include <cstdio>
static FILE* open_file(const std::string& filename)
{
    size_t siz = filename.size();
    if (filename[siz-1] == '|') {
        return popen(filename.substr(0,siz-1).c_str(), "r");
    }
    else {
        return fopen(filename.c_str(), "r");
    }
    return 0;
}


static void close_file(FILE* fp, const std::string& filename)
{
    size_t siz = filename.size();
    if (filename[siz-1] == '|') {
        // handle the residual lines;
        char buf[1024] = {'\0'};
        while (fgets(buf,1024,fp)) {
        }
        pclose(fp);
    }
    else {
        fclose(fp);
    }
}

static bool is_data(const char* line)
{
    int numchkindx = 0;      // C/C++ generator output
    if (line[0] == ' ') numchkindx = 1; // Fortran output
    return line[numchkindx] >= '0' && line[numchkindx] <= '9';
}

static bool get_line(FILE* fp, std::string& line)
{
    char buf[1024] = {'\0'};

    while (true) {
        if (!fgets(buf,1024,fp)) {
            return false;
        }

        if (buf[1022]) {
            return false;          // Too large line, treat as error
        }

        if (is_data(buf)) {
            break;
        }
    }

    line = buf;
    return true;
}

static bool unpack(const std::string& line, HepMC::GenEvent& event)
{
    int status=0;     // status code
    int pdgid=0;      // HEP PDG code
    int oldest_daughter=0;    // first daughter
    int baby_daughter=0;    // last daughter
    double px=0.0; // px in GeV
    double py=0.0; // py in GeV
    double pz=0.0; // pz in GeV
    double req_novalue=1e30; // larger than the diameter of univers in mm
    double mass=req_novalue; // mass in GeV
    double vertexX=req_novalue; // x vertex in mm requested on this line
    double vertexY=req_novalue; // y vertex in mm "
    double vertexZ=req_novalue; // z vertex in mm "
    double vertex_dT=req_novalue; // vertex _delta_ time (in ns)
    double polx=0.0;  // x polarization
    double poly=0.0;  // y polarization
    double polz=0.0;  // z polarization
    double energy_unit= CLHEP::GeV;
    double position_unit= CLHEP::millimeter;
    double time_unit= CLHEP::nanosecond; /* used to be mm/c_light */

    std::istringstream is;
    is.str(line);
    is >> status >> pdgid >> oldest_daughter >> baby_daughter
       >> px >> py >> pz >> mass
       >> vertex_dT >> vertexX >> vertexY >> vertexZ // note order, T first
       >> polx >> poly >> polz;
        
    
    HepMC::GenParticle* particle = new HepMC::GenParticle
        (HepMC::FourVector(px*energy_unit,
                          py*energy_unit,
                          pz*energy_unit,
                          sqrt(px*px+py*py+pz*pz+mass*mass)*energy_unit),
         pdgid,status);
    HepMC::GenVertex* vertex = new HepMC::GenVertex
        (HepMC::FourVector(vertexX*position_unit,
                          vertexY*position_unit,
                          vertexZ*position_unit,
                          vertex_dT*time_unit));
    event.add_vertex(vertex);
    if (status == 1 || status == 0x1000 || status == 0x2000) {
        /*
          status=0x1000 : use G4 to do the radioactive decay. 
          status=0x2000 : use G4 to do the radioactive decay and use the rigth life time.
          relevant code is in : Simulation/DetSimV2/DetSimOptions/src/LSExpPrimaryGeneratorAction.cc
				 
        */
        vertex->add_particle_out(particle);
        event.set_signal_process_vertex(vertex);
    } else {
        vertex->add_particle_in(particle);
    }
    return true;
}


bool HepEvt2HepMC::fill(HepMC::GenEvent& event)
{
    if (!m_fp) {
        return false;
    }

    std::string line;
    int nlines = 0;
    while (get_line(m_fp,line)) {
        std::istringstream is; 
        is.str(line);
        is >> nlines;
        
        for(; nlines; --nlines) {
            get_line(m_fp,line);

            if (!unpack(line,event)) return false;
        }

        ++m_eventCount;
        break;
    }
    return true;
}


