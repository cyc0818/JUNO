#ifndef IGenTool_h
#define IGenTool_h

namespace HepMC {
    class GenEvent;
}

class IGenTool {
    public:
        virtual bool configure()=0;
        virtual bool mutate(HepMC::GenEvent& event)=0;
        virtual ~IGenTool();
};

#endif
