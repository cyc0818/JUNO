#ifndef TtLikelihood_h
#define TtLikelihood_h

#include "Event/TtCalibEvt.h"
#include "Geometry/TtGeom.h"

#include<list>
#include<string>
#include<map>

class TtLikelihoodBase {
    public:
        TtLikelihoodBase(const std::list<JM::CalibTtChannel*> * data);
        ~TtLikelihoodBase();

        virtual float check_fit(int npar, double * pars) =0;
        virtual std::map<std::string,float> * extra_info() =0;

        void SetGeometry(TtGeom * ttGeom);

    protected:
        const std::list<JM::CalibTtChannel*> * m_data;
        TtGeom * m_ttGeom;
};

class TtLH_HitNoHit : public TtLikelihoodBase {
    public:
        TtLH_HitNoHit(const std::list<JM::CalibTtChannel*> * data);
        float check_fit(int npar, double * pars);
        std::map<std::string,float> * extra_info();

        std::vector<unsigned int> * getHitStrips   (){ return &hit_strips   ; }
        std::vector<unsigned int> * getMiss1SStrips(){ return &miss1s_strips; }
        std::vector<unsigned int> * getMissedStrips(){ return &missed_strips; }
        std::vector<unsigned int> * getNoiseStrips (){ return &noise_strips ; }

        enum topoTypes{
            kUnknown=0,
            kRegular=1,
            kChimney=2,
            ktopoBits=2
        };
        int getTopoType();

        std::map<unsigned int, float> * getDistHit2PMT(){ return &dist_hit_to_pmt ; }
    private:
        int n_hit_strips;
        int n_missed_strips;
        int n_missed1side_strips;
        int n_noise_strips;

        std::vector<unsigned int> hit_strips;
        std::vector<unsigned int> miss1s_strips;
        std::vector<unsigned int> missed_strips;
        std::vector<unsigned int> noise_strips;

        std::map<unsigned int, float> dist_hit_to_pmt;
};

class TtLH_TimeCheck : public TtLikelihoodBase {
    public:
        struct hit_time_info{
            float distance_in_strip;
            float distance_muon_to_strip;
            float time_propagation;
            float hit_time;
            float corr_time;
        };

        TtLH_TimeCheck(const std::list<JM::CalibTtChannel*> * data);
        float check_fit(int npar, double * pars);
        std::map<std::string,float> * extra_info();

        std::map<unsigned int, struct hit_time_info> * getTimeInfo(){ return &time_info ; }
    private:
        int n_hit_strips;
        std::map<unsigned int, struct hit_time_info> time_info;
};

#endif  // TtLikelihood_h
// vim: expandtab shiftwidth=4 softtabstop=4 tabstop=4
