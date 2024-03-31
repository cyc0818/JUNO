
AUX_SOURCE_DIRECTORY(src src_GenSim_list)
add_library (GenSim SHARED
    ${src_GenSim_list}
)

target_link_libraries (GenSim
    G4global G4materials G4geometry
    CLHEP
    # root
    ${ROOT_LIBRARIES}
)

target_include_directories (GenSim
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_executable (EnergySpectrum
    apps/EnergySpectrum.cc
)

target_link_libraries(EnergySpectrum
    GenSim
)

