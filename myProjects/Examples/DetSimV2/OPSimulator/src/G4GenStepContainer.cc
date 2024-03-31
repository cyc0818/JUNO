#include "G4GenStepContainer.h"

#include <vector>

namespace opsimulator {

    bool G4GenStepContainer::clear() {
        m_eventids.clear();

        m_trackids.clear();
        m_pdgcodes.clear();
        m_tracksts.clear();
        m_matnames.clear();

        m_stepnos.clear();
        m_edeps.clear();
        m_eviss.clear();
        m_steplens.clear();

        m_pre_xs.clear();
        m_pre_ys.clear();
        m_pre_zs.clear();
        m_pre_ts.clear();
        m_pre_velocitys.clear();

        m_post_xs.clear();
        m_post_ys.clear();
        m_post_zs.clear();
        m_post_ts.clear();
        m_post_velocitys.clear();

        m_delta_xs.clear();
        m_delta_ys.clear();
        m_delta_zs.clear();

        return true;
    }

    int G4GenStepContainer::size() {
        return m_edeps.size(); // assume their sizes are equal.
    }

    int G4GenStepContainer::new_entry() {
        m_eventids.push_back(0);

        m_trackids.push_back(0);
        m_pdgcodes.push_back(0);
        m_tracksts.push_back(0);
        m_matnames.push_back("");

        m_stepnos.push_back(0);
        m_edeps.push_back(0);
        m_eviss.push_back(0);
        m_steplens.push_back(0);

        m_pre_xs.push_back(0);
        m_pre_ys.push_back(0);
        m_pre_zs.push_back(0);
        m_pre_ts.push_back(0);
        m_pre_velocitys.push_back(0);

        m_post_xs.push_back(0);
        m_post_ys.push_back(0);
        m_post_zs.push_back(0);
        m_post_ts.push_back(0);
        m_post_velocitys.push_back(0);

        m_delta_xs.push_back(0);
        m_delta_ys.push_back(0);
        m_delta_zs.push_back(0);

        return (int)m_stepnos.size()-1;
    }

    // = Event Level =
    int G4GenStepContainer::eventid(int idx) {
        return m_eventids[idx];
    }

    void G4GenStepContainer::eventid(int idx, int val) {
        m_eventids[idx] = val;
    }

    // = Track Level =
    int G4GenStepContainer::trackid(int idx) {
        return m_trackids[idx];
    }

    void G4GenStepContainer::trackid(int idx, int val) {
        m_trackids[idx] = val;
    }

    int G4GenStepContainer::pdgcode(int idx) {
        return m_pdgcodes[idx];
    }

    void G4GenStepContainer::pdgcode(int idx, int val) {
        m_pdgcodes[idx] = val;
    }

    int G4GenStepContainer::trackst(int idx) {
        return m_tracksts[idx];
    }

    void G4GenStepContainer::trackst(int idx, int val) {
        m_tracksts[idx] = val;
    }

    const std::string& G4GenStepContainer::matname(int idx) {
        return m_matnames[idx];
    }
    
    void G4GenStepContainer::matname(int idx, const std::string& val) {
        m_matnames[idx] = val;
    }

    // = Step Level =

    int G4GenStepContainer::stepno(int idx) {
        return m_stepnos[idx];
    }

    void G4GenStepContainer::stepno(int idx, int val) {
        m_stepnos[idx] = val;
    }

    double G4GenStepContainer::edep(int idx) {
        return m_edeps[idx];
    }

    void G4GenStepContainer::edep(int idx, double val) {
        m_edeps[idx] = val;
    }

    double G4GenStepContainer::evis(int idx) {
        return m_eviss[idx];
    }

    void G4GenStepContainer::evis(int idx, double val) {
        m_eviss[idx] = val;
    }

    double G4GenStepContainer::steplen(int idx) {
        return m_steplens[idx];
    }

    void G4GenStepContainer::steplen(int idx, double val) {
        m_steplens[idx] = val;
    }

    // pre_x/y/z/t/velocity
    double G4GenStepContainer::pre_x(int idx) {
        return m_pre_xs[idx];
    }

    void G4GenStepContainer::pre_x(int idx, double val) {
        m_pre_xs[idx] = val;
    }

    double G4GenStepContainer::pre_y(int idx) {
        return m_pre_ys[idx];
    }

    void G4GenStepContainer::pre_y(int idx, double val) {
        m_pre_ys[idx] = val;
    }

    double G4GenStepContainer::pre_z(int idx) {
        return m_pre_zs[idx];
    }

    void G4GenStepContainer::pre_z(int idx, double val) {
        m_pre_zs[idx] = val;
    }

    double G4GenStepContainer::pre_t(int idx) {
        return m_pre_ts[idx];
    }

    void G4GenStepContainer::pre_t(int idx, double val) {
        m_pre_ts[idx] = val;
    }

    double G4GenStepContainer::pre_velocity(int idx) {
        return m_pre_velocitys[idx];
    }

    void G4GenStepContainer::pre_velocity(int idx, double val) {
        m_pre_velocitys[idx] = val;
    }

    // post_x/y/z/t/velocity
    double G4GenStepContainer::post_x(int idx) {
        return m_post_xs[idx];
    }

    void G4GenStepContainer::post_x(int idx, double val) {
        m_post_xs[idx] = val;
    }

    double G4GenStepContainer::post_y(int idx) {
        return m_post_ys[idx];
    }

    void G4GenStepContainer::post_y(int idx, double val) {
        m_post_ys[idx] = val;
    }

    double G4GenStepContainer::post_z(int idx) {
        return m_post_zs[idx];
    }

    void G4GenStepContainer::post_z(int idx, double val) {
        m_post_zs[idx] = val;
    }

    double G4GenStepContainer::post_t(int idx) {
        return m_post_ts[idx];
    }

    void G4GenStepContainer::post_t(int idx, double val) {
        m_post_ts[idx] = val;
    }

    double G4GenStepContainer::post_velocity(int idx) {
        return m_post_velocitys[idx];
    }

    void G4GenStepContainer::post_velocity(int idx, double val) {
        m_post_velocitys[idx] = val;
    }

    // delta_x/y/z
    double G4GenStepContainer::delta_x(int idx) {
        return m_delta_xs[idx];
    }

    void G4GenStepContainer::delta_x(int idx, double val) {
        m_delta_xs[idx] = val;
    }

    double G4GenStepContainer::delta_y(int idx) {
        return m_delta_ys[idx];
    }

    void G4GenStepContainer::delta_y(int idx, double val) {
        m_delta_ys[idx] = val;
    }

    double G4GenStepContainer::delta_z(int idx) {
        return m_delta_zs[idx];
    }

    void G4GenStepContainer::delta_z(int idx, double val) {
        m_delta_zs[idx] = val;
    }


} // namespace opsimulator

