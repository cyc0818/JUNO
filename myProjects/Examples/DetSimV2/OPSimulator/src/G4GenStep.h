#ifndef G4GenStep_h
#define G4GenStep_h

/*
 * G4GenStep:
 *   Implementation of GenStep to work with G4 engine.
 * 
 * In order to decouple with the underlying data structures, only the index is stored.
 * The container is responsible for locating an element using the index.
 *
 */

#include "OPSimulator/IGenStep.h"

namespace opsimulator {

template <typename Container>
struct G4GenStep: IGenStep {

    G4GenStep(Container& c)
        : m_container(c) {
        m_idx = m_container.new_entry();
    }

    //  = Event Level =
    virtual int eventid() {
        return m_container.eventid(m_idx);
    }
    virtual void eventid(int val) {
        m_container.eventid(m_idx, val);
    }

    // = Track Level =
    virtual int trackid() {
        return m_container.trackid(m_idx);
    }
    virtual void trackid(int val) {
        m_container.trackid(m_idx, val);
    }

    virtual int pdgcode() {
        return m_container.pdgcode(m_idx);
    }
    virtual void pdgcode(int val) {
        m_container.pdgcode(m_idx, val);
    }

    virtual int trackst() {
        return m_container.trackst(m_idx);
    }
    virtual void trackst(int val) {
        m_container.trackst(m_idx, val);
    }

    virtual const std::string& matname() {
        return m_container.matname(m_idx);
    }
    virtual void matname(const std::string& val) {
        m_container.matname(m_idx, val);
    }

    //  = Step Level =

    virtual int stepno() {
        return m_container.stepno(m_idx);
    }
    virtual void stepno(int val) {
        m_container.stepno(m_idx, val);
    }

    virtual double edep() {
        return m_container.edep(m_idx);
    }
    virtual void edep(double val) {
        m_container.edep(m_idx, val);
    }

    virtual double evis() {
        return m_container.evis(m_idx);
    }
    virtual void evis(double val) {
        m_container.evis(m_idx, val);
    }

    virtual double steplen() {
        return m_container.steplen(m_idx);
    }
    virtual void steplen(double val) {
        m_container.steplen(m_idx, val);
    }

    // pre_x/y/z/t/velocity
    virtual double pre_x() {
        return m_container.pre_x(m_idx);
    }
    virtual void pre_x(double val) {
        m_container.pre_x(m_idx, val);
    }

    virtual double pre_y() {
        return m_container.pre_y(m_idx);
    }
    virtual void pre_y(double val) {
        m_container.pre_y(m_idx, val);
    }

    virtual double pre_z() {
        return m_container.pre_z(m_idx);
    }
    virtual void pre_z(double val) {
        m_container.pre_z(m_idx, val);
    }

    virtual double pre_t() {
        return m_container.pre_t(m_idx);
    }
    virtual void pre_t(double val) {
        m_container.pre_t(m_idx, val);
    }

    virtual double pre_velocity() {
        return m_container.pre_velocity(m_idx);
    }
    virtual void pre_velocity(double val) {
        m_container.pre_velocity(m_idx, val);
    }

    // post_x/y/z/t/velocity
    virtual double post_x() {
        return m_container.post_x(m_idx);
    }
    virtual void post_x(double val) {
        m_container.post_x(m_idx, val);
    }

    virtual double post_y() {
        return m_container.post_y(m_idx);
    }
    virtual void post_y(double val) {
        m_container.post_y(m_idx, val);
    }

    virtual double post_z() {
        return m_container.post_z(m_idx);
    }
    virtual void post_z(double val) {
        m_container.post_z(m_idx, val);
    }

    virtual double post_t() {
        return m_container.post_t(m_idx);
    }
    virtual void post_t(double val) {
        m_container.post_t(m_idx, val);
    }

    virtual double post_velocity() {
        return m_container.post_velocity(m_idx);
    }
    virtual void post_velocity(double val) {
        m_container.post_velocity(m_idx, val);
    }

    // delta_x/y/z
    virtual double delta_x() {
        return m_container.delta_x(m_idx);
    }
    virtual void delta_x(double val) {
        m_container.delta_x(m_idx, val);
    }

    virtual double delta_y() {
        return m_container.delta_y(m_idx);
    }
    virtual void delta_y(double val) {
        m_container.delta_y(m_idx, val);
    }

    virtual double delta_z() {
        return m_container.delta_z(m_idx);
    }
    virtual void delta_z(double val) {
        m_container.delta_z(m_idx, val);
    }

private:
    int m_idx;
    Container& m_container;
};

} // namespace opsimulator

#endif
