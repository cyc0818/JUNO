
add_subdirectory(InverseBeta)
add_subdirectory(Muon)

add_subdirectory(NuSolGen)
add_subdirectory(Supernova)

add_subdirectory(RadioActivity)

add_subdirectory(GeoNu)

add_subdirectory(AtmNuGen)
add_subdirectory(ProtonDecay)
add_subdirectory(ReactorNuES)

add_subdirectory(GenGenie)

add_subdirectory(UnifiedGen)
add_subdirectory(GenDeex)

# only apply in case NuWro_Found is TRUE
# in this case we can assume a downstream
# patched NuWro is present in external libs
if(NuWro_Found)
  add_subdirectory(GenNuWro)
endif()
