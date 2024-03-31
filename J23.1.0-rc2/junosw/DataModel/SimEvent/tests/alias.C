
void SimEventAlias(TTree* SimEvent) {

    // create alias to simplify analysis
    // = Track =
    // == ID ==
    SimEvent->SetAlias("pdg_id", "m_tracks.pdg_id");
    SimEvent->SetAlias("track_id", "m_tracks.track_id");
    // == Initial Mass, Momentum, Position ==
    SimEvent->SetAlias("init_mass", "m_tracks.init_mass");
    SimEvent->SetAlias("init_px", "m_tracks.init_px");
    SimEvent->SetAlias("init_py", "m_tracks.init_py");
    SimEvent->SetAlias("init_pz", "m_tracks.init_pz");
    SimEvent->SetAlias("init_mom", "(init_px**2+init_py**2+init_pz**2)**0.5");
    SimEvent->SetAlias("init_kine", "(init_mom**2+init_mass**2)**0.5-init_mass");

    SimEvent->SetAlias("init_x", "m_tracks.init_x");
    SimEvent->SetAlias("init_y", "m_tracks.init_y");
    SimEvent->SetAlias("init_z", "m_tracks.init_z");
    SimEvent->SetAlias("init_r", "(init_x**2+init_y**2+init_z**2)**0.5");
    SimEvent->SetAlias("init_r3", "(init_r)**3");
    SimEvent->SetAlias("init_t", "m_tracks.init_t");
    // == Exit Mass, Momentum, Position ==
    SimEvent->SetAlias("exit_px", "m_tracks.exit_px");
    SimEvent->SetAlias("exit_py", "m_tracks.exit_py");
    SimEvent->SetAlias("exit_pz", "m_tracks.exit_pz");
    SimEvent->SetAlias("exit_mom", "(exit_px**2+exit_py**2+exit_pz**2)**0.5");
    SimEvent->SetAlias("exit_kine", "(exit_mom**2+init_mass**2)**0.5-init_mass");

    SimEvent->SetAlias("exit_x", "m_tracks.exit_x");
    SimEvent->SetAlias("exit_y", "m_tracks.exit_y");
    SimEvent->SetAlias("exit_z", "m_tracks.exit_z");
    SimEvent->SetAlias("exit_t", "m_tracks.exit_t");

    SimEvent->SetAlias("track_length", "m_tracks.track_length");
    // == Deposit and Quenched Energy ==
    SimEvent->SetAlias("edep",   "m_tracks.edep");
    SimEvent->SetAlias("edep_x", "m_tracks.edep_x");
    SimEvent->SetAlias("edep_y", "m_tracks.edep_y");
    SimEvent->SetAlias("edep_z", "m_tracks.edep_z");
    SimEvent->SetAlias("edep_r", "(edep_x**2+edep_y**2+edep_z**2)**0.5");
    SimEvent->SetAlias("edep_r3", "(edep_r)**3");

    SimEvent->SetAlias("Qedep",   "m_tracks.Qedep");
    SimEvent->SetAlias("Qedep_x", "m_tracks.Qedep_x");
    SimEvent->SetAlias("Qedep_y", "m_tracks.Qedep_y");
    SimEvent->SetAlias("Qedep_z", "m_tracks.Qedep_z");
    SimEvent->SetAlias("Qedep_r", "(Qedep_x**2+Qedep_y**2+Qedep_z**2)**0.5");
    SimEvent->SetAlias("Qedep_r3", "(Qedep_r)**3");

    SimEvent->SetAlias("edep_notinLS", "m_tracks.edep_notinLS");
    // = CD Hits =
    /* Please note, The hits can be merged.
     * If you want to draw the variable below directly, 
     * please add the weight ``npe''. 
     */
    // == PMT ID ==
    SimEvent->SetAlias("pmtid", "m_cd_hits.pmtid");
    SimEvent->SetAlias("npe", "m_cd_hits.npe");
    SimEvent->SetAlias("hittime", "m_cd_hits.hittime");
}
