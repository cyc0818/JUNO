
include(CMakePackageConfigHelpers)

configure_package_config_file(cmake/offlineConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                              INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
                              )

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/")

install(EXPORT ${PROJECT_NAME}Targets
        NAMESPACE ${PROJECT_NAME}::
        FILE "${PROJECT_NAME}Targets.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/")

### Setup scripts
include("${CMAKE_CURRENT_LIST_DIR}/JUNOInstallOfflineSetup.cmake")

### Install the static macros ###
install(FILES "cmake/legacy/Modules/PKG.cmake"
              "cmake/legacy/Modules/EDM.cmake"
              "cmake/legacy/Modules/XOD.cmake"
              "cmake/legacy/Modules/RootDict.cmake"
              "cmake/legacy/JUNODependencies.cmake"
              "cmake/legacy/JUNOOnlineDependencies.cmake"
              "cmake/legacy/JUNOVersionDefinition.cmake"
              "cmake/legacy/JUNOSetupScripts.cmake"
              "cmake/legacy/JUNOCommonDefinitions.cmake"
              "cmake/legacy/JUNOInstallOfflineSetup.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/")
