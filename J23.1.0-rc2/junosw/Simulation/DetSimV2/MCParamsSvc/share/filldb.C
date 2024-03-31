TString load(TString param) {
    TString result;

    const char* junotop = gSystem->Getenv("JUNOTOP");
    if (!junotop) {
        std::cerr << "can't find JUNOTOP" << std::endl;
        return result;
    }

    TString fn = TString(junotop) + "/data/Simulation/DetSim/";
    param.ReplaceAll(".", "/");

    fn += param;

    ifstream ifs (fn.Data());
    string s;
    getline (ifs, s, (char) ifs.eof());

    result = TBase64::Encode(s.c_str());

    return result;
}

void filldb() {

    const char *ins = "INSERT INTO Material_LS ("
        " create_by, "
        " create_date, remarks, del_flag, version, "
        " RINDEX, ABSLENGTH, ABSLENGTHwithUnits, "
        " FASTCOMPONENT, REEMISSIONPROB, RAYLEIGH, "
        " SCINTILLATIONYIELD, RESOLUTIONSCALE, "
        " GammaFASTTIMECONSTANT, GammaSLOWTIMECONSTANT, GammaYIELDRATIO, "
        " AlphaFASTTIMECONSTANT, AlphaSLOWTIMECONSTANT, AlphaYIELDRATIO, "
        " NeutronFASTTIMECONSTANT, NeutronSLOWTIMECONSTANT, NeutronYIELDRATIO, "
        " scale "
        " ) VALUES ('%s'," // id, create_by
        " '%s', '%s', '%s', '%s', " // create_date, remarks, del_flag, version
        " '%s', '%s', '%s', " // RINDEX, ABSLENGTH, ABSLENGTH units
        " '%s', '%s', '%s', " // FASTCOMPONENT, REEMISSIONPROB, RAYLEIGH,
        " '%s', '%s',       " // SCINTILLATIONYIELD, RESOLUTIONSCALE
        " '%s', '%s', '%s', " // Gamma
        " '%s', '%s', '%s', " // Alpha
        " '%s', '%s', '%s', " // Neutron
        " '%s')"; // scale

    // open connection to MySQL server on localhost
    TSQLServer *db = TSQLServer::Connect("mysql://junodb1.ihep.ac.cn/offline_db", "username", "password");
    TSQLResult *res = 0;

    TString r;

    TString ridx     = load("Material.LS.RINDEX");
    TString abslen   = load("Material.LS.ABSLENGTH");
    TString abslenu  = load("Material.LS.ABSLENGTHwithUnits");
    TString fastc    = load("Material.LS.FASTCOMPONENT");
    TString reem     = load("Material.LS.REEMISSIONPROB");
    TString rayleigh = load("Material.LS.RAYLEIGH");
    TString lsyield  = load("Material.LS.SCINTILLATIONYIELD");
    TString rs       = load("Material.LS.RESOLUTIONSCALE");
    TString gf       = load("Material.LS.GammaFASTTIMECONSTANT");
    TString gs       = load("Material.LS.GammaSLOWTIMECONSTANT");
    TString gy       = load("Material.LS.GammaYIELDRATIO");
    TString af       = load("Material.LS.AlphaFASTTIMECONSTANT");
    TString as       = load("Material.LS.AlphaSLOWTIMECONSTANT");
    TString ay       = load("Material.LS.AlphaYIELDRATIO");
    TString nf       = load("Material.LS.NeutronFASTTIMECONSTANT");
    TString ns       = load("Material.LS.NeutronSLOWTIMECONSTANT");
    TString ny       = load("Material.LS.NeutronYIELDRATIO");
    TString scale    = load("Material.LS.scale");
    

    r.Form(
           ins,
           "lintao",  // id, create_by
           "2018-07-19", "test", "0", "trunk", // create_date, remarks, del_flag, version
           ridx.Data(), abslen.Data(), abslenu.Data(), // RINDEX, ABSLENGTH, ABSLENGTH units
           fastc.Data(), reem.Data(), rayleigh.Data(), // FASTCOMPONENT, REEMISSIONPROB, RAYLEIGH,
           lsyield.Data(), rs.Data(), // SCINTILLATIONYIELD, RESOLUTIONSCALE
           gf.Data(), gs.Data(), gy.Data(), // Gamma
           af.Data(), as.Data(), ay.Data(), // Alpha
           nf.Data(), ns.Data(), ny.Data(), // Neutron
           scale.Data()); // scale                  

    std::cout << r << std::endl;

    res = db->Query(r);
    delete res;

}
