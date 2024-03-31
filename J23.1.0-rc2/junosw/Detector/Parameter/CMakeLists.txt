
PKG(Parameter
    DEPENDS
        Boost::filesystem Boost::system
        Rest
        ${GEANT4_LIBRARIES}
    SETENV
        JUNO_PARAMETER_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
)
