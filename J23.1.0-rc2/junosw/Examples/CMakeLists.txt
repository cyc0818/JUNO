# PRJ(Examples
#     DEPENDS
#         DataModel
#         Simulation
#         Reconstruction
# )

add_subdirectory(Tutorial)
add_subdirectory(FirstAlg)
add_subdirectory(CorAnalysis)
add_subdirectory(ElecAnalysis)
add_subdirectory(MCTruthAnalysis)
add_subdirectory(RawAnalysis)
add_subdirectory(PSDTag)

if (ONNXRUNTIME_FOUND)
  add_subdirectory(ONNXRuntimeInference)
endif()
