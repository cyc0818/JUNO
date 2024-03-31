PKG(SphMuonRecTool
    DEPENDS
      BufferMemMgr
      RecTools
      gsl
      Spectrum
#      -fopenmp
    SETENV
      RECMU_SPHFUNC_DATA="$ENV{JUNOTOP}/data/Reconstruction/RecMuon/SphMuonData"
)
