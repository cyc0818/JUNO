#ifndef G4GenStepContainer_h
#define G4GenStepContainer_h

/*
 * G4GenStepContainer:
 *   An implementation of container for genstep.
 *
 *   The data are stored in a list of vectors.
 *   This is an example to show the different data organization.
 *
 */

#include <vector>
#include <string>

namespace opsimulator {

struct G4GenStepContainer {
    // clear
    bool clear();

    // query size
    int size();

    // create a new entry
    int new_entry();

    // = Event Level =
    int eventid(int idx);
    void eventid(int idx, int val);

    // = Track Level =

    int trackid(int idx);
    void trackid(int idx, int val);

    int pdgcode(int idx);
    void pdgcode(int idx, int val);

    int trackst(int idx);
    void trackst(int idx, int val);

    const std::string& matname(int idx);
    void matname(int idx, const std::string& val);

    // = Step Level =

    int stepno(int idx);
    void stepno(int idx, int val);

    double edep(int idx);
    void edep(int idx, double val);

    double evis(int idx);
    void evis(int idx, double val);

    double steplen(int idx);
    void steplen(int idx, double val);

    // pre_x/y/z/t/velocity
    double pre_x(int idx);
    void pre_x(int idx, double val);

    double pre_y(int idx);
    void pre_y(int idx, double val);

    double pre_z(int idx);
    void pre_z(int idx, double val);

    double pre_t(int idx);
    void pre_t(int idx, double val);

    double pre_velocity(int idx);
    void pre_velocity(int idx, double val);

    // post_x/y/z/t/velocity
    double post_x(int idx);
    void post_x(int idx, double val);

    double post_y(int idx);
    void post_y(int idx, double val);

    double post_z(int idx);
    void post_z(int idx, double val);

    double post_t(int idx);
    void post_t(int idx, double val);

    double post_velocity(int idx);
    void post_velocity(int idx, double val);

    // delta_x/y/z
    double delta_x(int idx);
    void delta_x(int idx, double val);

    double delta_y(int idx);
    void delta_y(int idx, double val);

    double delta_z(int idx);
    void delta_z(int idx, double val);

    // pre_beta
    double pre_beta(int idx);
    void pre_beta(int idx, double val);

    // post_beta
    double post_beta(int idx);
    void post_beta(int idx, double val);

    // num_ceren
    double num_ceren(int idx);
    void num_ceren(int idx, double val);

    double num_ceren_1(int idx);
    void num_ceren_1(int idx, double val);

    double num_ceren_2(int idx);
    void num_ceren_2(int idx, double val);

private:
    // = Event Level =
    std::vector<int> m_eventids;
    // = Track Level =
    std::vector<int> m_trackids;
    std::vector<int> m_pdgcodes;
    std::vector<int> m_tracksts;
    std::vector<std::string> m_matnames;
    // = Step Level = 
    std::vector<int> m_stepnos;
    std::vector<double> m_edeps;
    std::vector<double> m_eviss;
    std::vector<double> m_steplens;
    // pre step
    std::vector<double> m_pre_xs;
    std::vector<double> m_pre_ys;
    std::vector<double> m_pre_zs;
    std::vector<double> m_pre_ts;
    std::vector<double> m_pre_velocitys;
    // post step
    std::vector<double> m_post_xs;
    std::vector<double> m_post_ys;
    std::vector<double> m_post_zs;
    std::vector<double> m_post_ts;
    std::vector<double> m_post_velocitys;
    // delta
    std::vector<double> m_delta_xs;
    std::vector<double> m_delta_ys;
    std::vector<double> m_delta_zs;

    // ceren
    std::vector<double> m_pre_betas;
    std::vector<double> m_post_betas;
    std::vector<double> m_num_cerens;
    std::vector<double> m_num_ceren_1s;
    std::vector<double> m_num_ceren_2s;

};

} // namespace opsimulator

#endif
