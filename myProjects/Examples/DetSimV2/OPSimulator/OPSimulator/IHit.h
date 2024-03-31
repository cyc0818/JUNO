#ifndef IHit_h
#define IHit_h

namespace opsimulator {

struct IHit {
    virtual int eventid() = 0;
    virtual int hitid() = 0;
};

} // namespace IHit

#endif
