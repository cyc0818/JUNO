
include (EDM)

EDM(ElecEvent
    FILES
      CdLpmtElecHeader
      CdSpmtElecHeader
      CdWaveformHeader
      WpElecHeader
      WpWaveformHeader
      TtElecHeader
      CdLpmtElecEvt
      CdSpmtElecEvt
      CdWaveformEvt
      WpElecEvt
      WpWaveformEvt
      TtElecEvt
      SpmtElecAbcBlock
      SpmtElecDiscrWord
      SpmtElecSpecialWord
      ElecWaveform
      ElecChannel
      CdTriggerHeader
      CdTriggerEvt
      WpTriggerHeader
      WpTriggerEvt
      TtTriggerHeader
      TtTriggerEvt
    DEPENDS
      Context
)
