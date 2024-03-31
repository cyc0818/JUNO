#include "EDMUtil/BookEDM.h"

// Generator and detector simulation
JUNO_BOOK_EDM(JM::GenHeader, JM::GenEvt, 100, /Event/Gen);
JUNO_BOOK_EDM(JM::SimHeader, JM::SimEvt, 200, /Event/Sim);
JUNO_BOOK_EDM(JM::MuonSimHeader, JM::MuonSimEvt, 210, /Event/MuonSim);

// Electronics simulation truth
JUNO_BOOK_EDM(JM::CdLpmtElecTruthHeader, JM::CdLpmtElecTruthEvt, 300, /Event/CdLpmtTruth);
JUNO_BOOK_EDM(JM::CdSpmtElecTruthHeader, JM::CdSpmtElecTruthEvt, 310, /Event/CdSpmtTruth);
JUNO_BOOK_EDM(JM::WpElecTruthHeader, JM::WpElecTruthEvt, 320, /Event/WpTruth);
JUNO_BOOK_EDM(JM::TtElecTruthHeader, JM::TtElecTruthEvt, 330, /Event/TtTruth);
JUNO_BOOK_EDM(JM::TrackElecTruthHeader, JM::TrackElecTruthEvt, 340, /Event/TrackTruth);

// Electronics simulation, trigger and raw data
JUNO_BOOK_EDM(JM::CdLpmtElecHeader, JM::CdLpmtElecEvt, 400, /Event/CdLpmtElec);
JUNO_BOOK_EDM(JM::CdSpmtElecHeader, JM::CdSpmtElecEvt, 410, /Event/CdSpmtElec);
JUNO_BOOK_EDM(JM::CdWaveformHeader, JM::CdWaveformEvt, 415, /Event/CdWaveform);
JUNO_BOOK_EDM(JM::WpElecHeader, JM::WpElecEvt, 420, /Event/WpElec);
JUNO_BOOK_EDM(JM::WpWaveformHeader, JM::WpWaveformEvt, 425, /Event/WpWaveform);
JUNO_BOOK_EDM(JM::TtElecHeader, JM::TtElecEvt, 430, /Event/TtElec);
JUNO_BOOK_EDM(JM::CdTriggerHeader, JM::CdTriggerEvt, 440, /Event/CdTrigger);
JUNO_BOOK_EDM(JM::WpTriggerHeader, JM::WpTriggerEvt, 445, /Event/WpTrigger);
JUNO_BOOK_EDM(JM::TtTriggerHeader, JM::TtTriggerEvt, 450, /Event/TtTrigger);

// Calibration
JUNO_BOOK_EDM(JM::CdLpmtCalibHeader, JM::CdLpmtCalibEvt, 500, /Event/CdLpmtCalib);
JUNO_BOOK_EDM(JM::CdSpmtCalibHeader, JM::CdSpmtCalibEvt, 510, /Event/CdSpmtCalib);
JUNO_BOOK_EDM(JM::WpCalibHeader, JM::WpCalibEvt, 520, /Event/WpCalib);
JUNO_BOOK_EDM(JM::TtCalibHeader, JM::TtCalibEvt, 530, /Event/TtCalib);

// Reconstruction
JUNO_BOOK_EDM(JM::CdVertexRecHeader, JM::CdVertexRecEvt, 600, /Event/CdVertexRec);
JUNO_BOOK_EDM(JM::CdTrackRecHeader, JM::CdTrackRecEvt, 605, /Event/CdTrackRec);
JUNO_BOOK_EDM(JM::WpRecHeader, JM::WpRecEvt, 610, /Event/WpRec);
JUNO_BOOK_EDM(JM::TtRecHeader, JM::TtRecEvt, 620, /Event/TtRec);


// Others
JUNO_BOOK_EDM(JM::PhyHeader, JM::PhyEvt, 700, /Event/Phy);
JUNO_BOOK_EDM(JM::TestHeaderA, JM::ATestEventA&JM::ATestEventB, 900, /Event/TestA);
JUNO_BOOK_EDM(JM::TestHeaderB, JM::BTestEventA&JM::BTestEventB, 1000, /Event/TestB);
