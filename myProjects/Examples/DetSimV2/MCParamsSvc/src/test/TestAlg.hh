#ifndef TestAlg_hh
#define TestAlg_hh

#include "SniperKernel/AlgBase.h"
#include "MCParamsSvc/IMCParamsSvc.hh"

class IMCParamsSvc;
class RootWriter;

class TestAlg: public AlgBase {
    public:
        TestAlg(const std::string& name);
        ~TestAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        // helper: save data into root ntuple
        typedef IMCParamsSvc::vec_d2d vec_d2d;
        void save_it(const std::string&, vec_d2d&);

    private:
        IMCParamsSvc* m_params_svc;
        RootWriter* m_rootwriter;
};

#endif
