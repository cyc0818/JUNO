#ifndef IGenStep_h
#define IGenStep_h

/*
 * A list of setters and getters.
 *
 * Event level
 * * eventid: Event ID
 *
 * Track level
 * * trackid: Track ID
 * * pdgcode: pdg of the particle
 * * trackst: track status
 * * matname: material name
 *
 * Step level
 * * stepno:  Step number in the track
 * * edep:    Deposit energy (MeV)
 * * steplen: Step Length (mm)
 * * pre_x/y/z/t: Pre-Step x/y/z/t (mm, ns)
 * * post_x/y/z/t: Post-Step x/y/z/t
 * * pre_velocity: velocity
 * * post_velocity: 
 * * delta_x/y/z: Delta position
 * 
 * Even though there are a lot of methods in the abstract class,
 * the implementation of GenStep is not forced to store all the data. 
 */

#include <string>

namespace opsimulator {

struct IGenStep {
    // = Event Level =
    virtual int eventid() = 0;
    virtual void eventid(int val) = 0;

    // = Track Level =

    virtual int trackid() = 0;
    virtual void trackid(int val) = 0;

    virtual int pdgcode() = 0;
    virtual void pdgcode(int val) = 0;

    virtual int trackst() = 0;
    virtual void trackst(int val) = 0;

    virtual const std::string& matname() = 0;
    virtual void matname(const std::string& val) = 0;

    // = Step Level =

    virtual int stepno() = 0;
    virtual void stepno(int val) = 0;

    virtual double edep() = 0;
    virtual void edep(double val) = 0;

    // visible energy (quenched)
    virtual double evis() = 0;
    virtual void evis(double val) = 0;

    virtual double steplen() = 0;
    virtual void steplen(double val) = 0;

    // pre_x/y/z/t/velocity
    virtual double pre_x() = 0;
    virtual void pre_x(double val) = 0;

    virtual double pre_y() = 0;
    virtual void pre_y(double val) = 0;

    virtual double pre_z() = 0;
    virtual void pre_z(double val) = 0;

    virtual double pre_t() = 0;
    virtual void pre_t(double val) = 0;

    virtual double pre_velocity() = 0;
    virtual void pre_velocity(double val) = 0;

    // post_x/y/z/t/velocity
    virtual double post_x() = 0;
    virtual void post_x(double val) = 0;

    virtual double post_y() = 0;
    virtual void post_y(double val) = 0;

    virtual double post_z() = 0;
    virtual void post_z(double val) = 0;

    virtual double post_t() = 0;
    virtual void post_t(double val) = 0;

    virtual double post_velocity() = 0;
    virtual void post_velocity(double val) = 0;

    // delta_x/y/z
    virtual double delta_x() = 0;
    virtual void delta_x(double val) = 0;

    virtual double delta_y() = 0;
    virtual void delta_y(double val) = 0;

    virtual double delta_z() = 0;
    virtual void delta_z(double val) = 0;

};

} // namespace opsimulator

#endif
