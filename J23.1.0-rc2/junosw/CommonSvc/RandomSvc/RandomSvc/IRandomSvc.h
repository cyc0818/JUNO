#ifndef IRandomSvc_h
#define IRandomSvc_h

class IRandomSvc {
public:
    virtual ~IRandomSvc() = 0;

    virtual double random() = 0;

    virtual long getSeed() = 0;
    virtual void setSeed(long seed) = 0;
};

#endif
