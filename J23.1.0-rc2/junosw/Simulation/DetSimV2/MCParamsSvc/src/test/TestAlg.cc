#include <boost/python.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <MCParamsSvc/IMCParamsSvc.hh>
#include "SniperKernel/AlgFactory.h"
#include "RootWriter/RootWriter.h"
#include <TTree.h>

#include <TestAlg.hh>

DECLARE_ALGORITHM(TestAlg);

TestAlg::TestAlg(const std::string& name)
    : AlgBase(name), m_params_svc(0), m_rootwriter(0)
{

}

TestAlg::~TestAlg()
{

}

bool
TestAlg::initialize()
{
    SniperPtr<IMCParamsSvc> mcgt(*getParent(), "MCParamsSvc");
    if (mcgt.invalid()) {
        LogError << "Can't find MCParamsSvc. " << std::endl;
        return false;
    }
    m_params_svc = mcgt.data();

    SniperPtr<RootWriter> rw(*getParent(), "RootWriter");
    if (rw.invalid()) {
        LogError << "Can't find RootWriter. " << std::endl;
        return false;
    }
    m_rootwriter = rw.data();
    
    return true;
}

bool
TestAlg::execute()
{
    bool st;
    IMCParamsSvc::vec_d2d LS_rindex;
    st = m_params_svc->Get("Material.LS.RINDEX", LS_rindex);
    if (st) { LogInfo << "LS.RINDEX: " << LS_rindex.size() << std::endl; }
    else { return false; }
    save_it("LS_RINDEX", LS_rindex);

    IMCParamsSvc::vec_d2d LS_abslen;
    st = m_params_svc->Get("Material.LS.ABSLENGTH", LS_abslen);
    if (st) { LogInfo << "LS.ABSLENGTH: " << LS_abslen.size() << std::endl; }
    else { return false; }
    save_it("LS_ABSLENGTH", LS_abslen);

    IMCParamsSvc::vec_d2d LS_abslen_with_units;
    st = m_params_svc->Get("Material.LS.ABSLENGTHwithUnits", LS_abslen_with_units);
    if (st) { LogInfo << "LS.ABSLENGTH with units: " << LS_abslen_with_units.size() << std::endl; }
    else { return false; }
    save_it("LS_ABSLENGTH", LS_abslen);

    IMCParamsSvc::vec_d2d LS_fastcomp;
    st = m_params_svc->Get("Material.LS.FASTCOMPONENT", LS_fastcomp);
    if (st) { LogInfo << "LS.FASTCOMPONENT: " << LS_fastcomp.size() << std::endl; }
    else { return false; }
    save_it("LS_FASTCOMPONENT", LS_fastcomp);

    IMCParamsSvc::vec_d2d LS_reemprob;
    st = m_params_svc->Get("Material.LS.REEMISSIONPROB", LS_reemprob);
    if (st) { LogInfo << "LS.REEMISSIONPROB: " << LS_reemprob.size() << std::endl; }
    else { return false; }
    save_it("LS_REEMISSIONPROB", LS_reemprob);

    IMCParamsSvc::vec_d2d LS_rayleigh;
    st = m_params_svc->Get("Material.LS.RAYLEIGH", LS_rayleigh);
    if (st) { LogInfo << "LS.RAYLEIGH: " << LS_rayleigh.size() << std::endl; }
    else { return false; }
    save_it("LS_RAYLEIGH", LS_rayleigh);

    IMCParamsSvc::vec_d2d LS_lightyield;
    st = m_params_svc->Get("Material.LS.SCINTILLATIONYIELD", LS_lightyield);
    if (st) { LogInfo << "LS.SCINTILLATIONYIELD: " << LS_lightyield[0].get<1>() << std::endl; }
    else { return false; }

    IMCParamsSvc::vec_d2d LS_resolutionscale;
    st = m_params_svc->Get("Material.LS.RESOLUTIONSCALE", LS_resolutionscale);
    if (st) { LogInfo << "LS.RESOLUTIONSCALE: " << LS_resolutionscale[0].get<1>() << std::endl; }
    else { return false; }


    IMCParamsSvc::vec_d2d LS_gamma_fasttime;
    IMCParamsSvc::vec_d2d LS_gamma_slowtime;
    IMCParamsSvc::vec_d2d LS_gamma_yieldratio;
    st = m_params_svc->Get("Material.LS.GammaFASTTIMECONSTANT", LS_gamma_fasttime);
    if (!st) { return false; }
    st = m_params_svc->Get("Material.LS.GammaSLOWTIMECONSTANT", LS_gamma_slowtime);
    if (!st) { return false; }
    st = m_params_svc->Get("Material.LS.GammaYIELDRATIO", LS_gamma_yieldratio);
    if (!st) { return false; }
    LogInfo << "LS gamma fast/slow/ratio: "
            << LS_gamma_fasttime[0].get<1>() << "/"
            << LS_gamma_slowtime[0].get<1>() << "/"
            << LS_gamma_yieldratio[0].get<1>() 
            << std::endl;

    IMCParamsSvc::vec_d2d LS_alpha_fasttime;
    IMCParamsSvc::vec_d2d LS_alpha_slowtime;
    IMCParamsSvc::vec_d2d LS_alpha_yieldratio;
    st = m_params_svc->Get("Material.LS.AlphaFASTTIMECONSTANT", LS_alpha_fasttime);
    if (!st) { return false; }
    st = m_params_svc->Get("Material.LS.AlphaSLOWTIMECONSTANT", LS_alpha_slowtime);
    if (!st) { return false; }
    st = m_params_svc->Get("Material.LS.AlphaYIELDRATIO", LS_alpha_yieldratio);
    if (!st) { return false; }
    LogInfo << "LS alpha fast/slow/ratio: "
            << LS_alpha_fasttime[0].get<1>() << "/"
            << LS_alpha_slowtime[0].get<1>() << "/"
            << LS_alpha_yieldratio[0].get<1>() 
            << std::endl;

    IMCParamsSvc::vec_d2d LS_neutron_fasttime;
    IMCParamsSvc::vec_d2d LS_neutron_slowtime;
    IMCParamsSvc::vec_d2d LS_neutron_yieldratio;
    st = m_params_svc->Get("Material.LS.NeutronFASTTIMECONSTANT", LS_neutron_fasttime);
    if (!st) { return false; }
    st = m_params_svc->Get("Material.LS.NeutronSLOWTIMECONSTANT", LS_neutron_slowtime);
    if (!st) { return false; }
    st = m_params_svc->Get("Material.LS.NeutronYIELDRATIO", LS_neutron_yieldratio);
    if (!st) { return false; }
    LogInfo << "LS neutron fast/slow/ratio: "
            << LS_neutron_fasttime[0].get<1>() << "/"
            << LS_neutron_slowtime[0].get<1>() << "/"
            << LS_neutron_yieldratio[0].get<1>() 
            << std::endl;

    // some scale factors are saved in string->double map
    IMCParamsSvc::vec_s2d LS_scales;
    st = m_params_svc->Get("Material.LS.scale", LS_scales);
    if (!st) { return false; }
    LogInfo << "LS scales: " << LS_scales.size() << std::endl;
    for (IMCParamsSvc::vec_s2d::iterator it = LS_scales.begin();
            it != LS_scales.end(); ++it) {
        LogInfo << " * " << *it << std::endl;
    }

    IMCParamsSvc::map_s2d LS_scales_map;
    st = m_params_svc->Get("Material.LS.scale", LS_scales_map);
    if (!st) { return false; }
    for (IMCParamsSvc::map_s2d::iterator it = LS_scales_map.begin();
            it != LS_scales_map.end(); ++it) {
        LogInfo << " * " << it->first << ": " << it->second << std::endl;
    }

    return true;
}

bool
TestAlg::finalize()
{
    return true;
}

void
TestAlg::save_it(const std::string& label, vec_d2d& props)
{

    int N = props.size();
    double* x = new double[N];
    double* y = new double[N];

    for (int i = 0; i < N; ++i) {
        x[i] = props[i].get<0>();
        y[i] = props[i].get<1>();
    }
    std::string name = "SIMEVT/";
    name += label;
    TTree* t = m_rootwriter->bookTree(*m_par, name, label);
    t->Branch("n", &N, "n/I");
    t->Branch("x", x, "x[n]/D");
    t->Branch("y", y, "y[n]/D");

    t->Fill();
}
