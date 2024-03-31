#ifndef FBRalg_H
#define FBRalg_H

//DEBUG MOD
#define HXDEBUG

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"
#include "Geometry/IPMTParamSvc.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Identifier/Identifier.h"
#include "Identifier/WpID.h"

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <math.h>
#include <tuple>
#define _USE_MATH_DEFINES
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"

#ifdef HXDEBUG
#endif

#ifdef PERFORMANCE_CHECK
#include "TTree.h"
#include "RootWriter/RootWriter.h"
#include "JunoTimer/IJunoTimerSvc.h"
#include "JunoTimer/JunoTimer.h"
#include "TSystem.h"
#endif

class FBRalg: public AlgBase{
    public:
        FBRalg(const std::string &name);
        ~FBRalg();

        bool initialize();
        bool execute();
        bool finalize();

        int WpID2pmtID(unsigned int pmtId)
        {
            Identifier id = Identifier(pmtId);
            int pmtid = WpID::module(id) + 30000;
            return pmtid;
        }
        std::tuple<double, double, double> Rot3D(double x, double y, double z, double theta, double phi)
        {
            theta = M_PI*theta/180;
            phi = M_PI*phi/180;
            double xx = std::cos(theta)*std::cos(phi)*x + std::cos(theta)*std::sin(phi)*y - std::sin(theta)*z;
            double yy = -std::sin(phi)*x + std::cos(phi)*y;
            double zz = std::sin(theta)*std::cos(phi)*x + std::sin(theta)*std::sin(phi)*y + std::cos(theta)*z;
            return std::make_tuple(xx, yy, zz);
        }
        std::tuple<double, double> ConAxis(double x, double y, double z)
        {
            double theta = std::acos(z/std::sqrt(x*x+y*y+z*z));
            double phi = std::atan(y/x);
            if(x < 0){
                if(y > 0){
                    phi = phi + M_PI;
                }
                else if(y < 0){
                    phi = phi - M_PI;
                }
            }
            if(phi < 0) phi = phi + 2*M_PI;
            return std::make_tuple(theta, phi);
        }

    private:
        double threQ = 3.;
        double Rf = 15000.;
        int i_nEvt;
        double Qbf;
        double wpmt_range;
        JM::NavBuffer *m_buf;
        IPMTParamSvc *m_pmtsvc;
        // --------------------------- Variables List
        int MaxWpPMTnum = 2400;
        std::vector<int> pmtid;
        std::vector<double> total_Q;
        std::tuple<double, double, double> check_xyz = std::make_tuple(0., 0., 0.);
        std::tuple<double, double> check_tp = std::make_tuple(0., 0.);
        std::tuple<double, double, double> vec = std::make_tuple(0., 0., 0.);

#ifdef PERFORMANCE_CHECK
        // for resources test
        TTree* m_fbralg;
        IJunoTimerSvc* m_junotimersvc;
        JunoTimerPtr m_timerevent;
        int N_mem;
        float mem_resident[4];//memory usage of the initialize and execute functions            
        float time_total;//time usage of the execute function    
#endif  
};

#endif
