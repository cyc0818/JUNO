# The order could be necessary

add_subdirectory(G4Svc)
add_subdirectory(DetSimAlg)
add_subdirectory(SimUtil)
add_subdirectory(MCGlobalTimeSvc)
add_subdirectory(MCParamsSvc)
add_subdirectory(PMTSim)
add_subdirectory(PhysiSim)
add_subdirectory(OPSimulator)
add_subdirectory(VoxelMethod)


# Detector Geometry Related
add_subdirectory(CalibUnit)
add_subdirectory(CentralDetector)
add_subdirectory(Chimney)
add_subdirectory(TopTracker)


# Applications
add_subdirectory(AnalysisCode)
add_subdirectory(DetSimOptions)


# Multi-threading
add_subdirectory(DetSimMT)
add_subdirectory(DetSimMTUtil)
