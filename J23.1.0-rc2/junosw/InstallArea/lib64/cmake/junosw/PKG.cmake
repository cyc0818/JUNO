### INCLUDE GUARD BEGIN ###
if(__offline_PKG_macro)
    return()
endif()
set(__offline_PKG_macro true)
### INCLUDE GUARD END ###

include(CMakeParseArguments)

#############################################################################
# PKG
# = PKGONLY =
# * Only placeholder, maybe with share
#
# = Build Algorithm =
# * By default, we assume the algorithm directory structure is:
#   + src
#   + python (optional)
#   + share (optional)
#   + test (optional)
#
# = Build Service =
# * By default, we assume the service directory structure is:
#   + PkgName (Header)
#   + src
#   + include (optional)
#   + python (optional)
#   + share (optional)
#   + test (optional)
#
# = Build Application =
# * Two cases:
#   + app only. Don't need libraries.
#   + app with lib. 
# 
#   APP app_name FILES app_src
#   APPONLY app_name
#
# = NOTE: build library as shared or module? =
#
# * In principle, all the SNiPER's dynamically-loadable elements should be
#   built as modules. 
# * The module could not be linked by other libraries.
# * Due to some reasons, the interface of service and the implementation is
#   not separated. This cause such libraries should be built as shared. 
# 
# The plan is to move to a more clean interface and a separate implemenataion.
# The default policy is building a 'shared' library.
# User can use option 'MODULE' to enable a 'module' library. 
# For consistency, 'SHARED' will be also provided. 
# It is also same to the CMake add_library. 
#
#############################################################################

function(PKG _package)

    ##########################################################################
    # = Prelude =
    ##########################################################################

    ##########################################################################
    # == the targets will be installed ==
    ##########################################################################
    # 
    # How it works?
    # - decalre several lists for bin/shlib targets 
    #   and include/python directories
    # - add the target/directory into one of the list
    # - create global property
    #   - ${_package}InstallBinTargets
    #   - ${_package}InstallShlibTargets
    #   - ${_package}InstallPythonDirs
    #   - ${_package}InstallIncludeDirs
    #     - the interface library should be also belong to this.
    # 
    set(_package_install_bin_targets)
    set(_package_install_shlib_targets)
    set(_package_install_python_dirs)
    set(_package_install_include_dirs)


    ##########################################################################
    # == cache package name ==
    ##########################################################################
    set_property(GLOBAL
        APPEND
        PROPERTY OFFLINE_PACKAGES "${_package}"
    )
    set_property(GLOBAL
        PROPERTY "${_package}_root" "${CMAKE_CURRENT_SOURCE_DIR}"
    )

    ##########################################################################
    # == depends ==
    ##########################################################################

    # == parse arguments ==
    ##########################################################################
    CMAKE_PARSE_ARGUMENTS(ARG "PKGONLY;NOAUX;MODULE" 
                              "APP;APPONLY" 
                              "FILES;DEPENDS;EXCLUDES;DICTS;SETENV" "" ${ARGN})
    ##########################################################################

    set(_other_srcs "${ARG_UNPARSED_ARGUMENTS}") # will not be used to generate dict
    if (_other_srcs)
        message(STATUS "${_package} will use addition files: ${_other_srcs}")
    endif()


    include_directories (${CMAKE_CURRENT_SOURCE_DIR})
    include_directories (src)
    include_directories (include)

    ##########################################################################
    # set the envvar
    ##########################################################################
    foreach(_kv ${ARG_SETENV})
        # split _kv by =
        string(REPLACE "=" ";" _envvar ${_kv})
        list(GET _envvar 0 _envvar_key)
        list(GET _envvar 1 _envvar_val)
        message(STRING "-> key: ${_envvar_key}")
        message(STRING "-> key: ${_envvar_val}")

        set_property(GLOBAL
            PROPERTY ${_envvar_key} ${_envvar_val}
        )
        set_property(GLOBAL
            APPEND
            PROPERTY ${_package}_extraenv ${_envvar_key}
        )

    endforeach()

    ##########################################################################
    # = Handle the denpendencies =
    # 
    # In order to build a package inside/outside offline, we need to handle 
    # following cases:
    #   1. the package is inside offline, so all the targets could be accessed
    #   2. the package is outside offline, the targets with offline::
    #      - In this case, check the targets with offline::
    ##########################################################################
    set (_depends_${package}_lists)

    foreach(_target ${ARG_DEPENDS})
        # message("--> ${_target}")
        if (TARGET ${_target})
            # message("----> ${_target}")
            list(APPEND _depends_${package}_lists ${_target})
        elseif(TARGET ${PROJECT_NAME}::${_target})
            # message("----> offline::${_target}")
            list(APPEND _depends_${package}_lists ${PROJECT_NAME}::${_target})
        elseif(TARGET junosw::${_target})
            # message("----> offline::${_target}")
            list(APPEND _depends_${package}_lists junosw::${_target})
        else()
            # message("------> FAILED TO FIND TARGET ${_target}, use ${_target}")
            list(APPEND _depends_${package}_lists ${_target})
        endif()
    endforeach()

    ##########################################################################
    # = Collect source code =
    ##########################################################################
    set (_src_${_package}_lists)

    if (ARG_NOAUX)
      message(STATUS "DISABLE AUX_SOURCE_DIRECTORY in ${_package}")

    else()
      AUX_SOURCE_DIRECTORY(src _src_${_package}_lists)
      ########################################################################
      # remove files in exclude list
      ########################################################################
      foreach (_f ${ARG_EXCLUDES})
        list (REMOVE_ITEM _src_${_package}_lists "${_f}")
      endforeach()
    endif(ARG_NOAUX)

    set(src_${_package}_lists)
    set(rootmap_${package}_list)
    set(pcm_${package}_list)
    
    ##########################################################################
    # remove files in _src_${_package}_lists 
    ##########################################################################
    foreach (_f ${_src_${_package}_lists})
        set(_index -1)
        if (ARG_FILES)
            list(FIND ARG_FILES "${_f}" _index)
        endif(ARG_FILES)

        if (${_index} GREATER -1)

        else()
            list(APPEND src_${_package}_lists "${_f}")
        endif()
    endforeach(_f)

    ##########################################################################
    # Dict generation
    ##########################################################################
    # Here, we assume header is given by user
    if (ARG_DICTS)
        FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Event 
                            ${CMAKE_CURRENT_BINARY_DIR}/src
                            ${CMAKE_CURRENT_BINARY_DIR}/xml
        )
        
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Event)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
        include_directories(${CMAKE_CURRENT_BINARY_DIR}/Event)
        include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)
        
        # Note: it's possible to use header under ${_package} directory,
        #       but it's better to avoid.
        # if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${_package})
        #     include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${_package})
        # endif()
        
        foreach (_f ${ARG_DICTS})
            set (_header)
            set (_linkdef)
            ##################################################################
            # Find the header
            ##################################################################

            if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_package}/${_f}.h")
                set (_header "${CMAKE_CURRENT_SOURCE_DIR}/${_package}/${_f}.h")
            elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Event/${_f}.h")
                set (_header "${CMAKE_CURRENT_SOURCE_DIR}/Event/${_f}.h")
            elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/${_f}.h")
                set (_header "${CMAKE_CURRENT_SOURCE_DIR}/include/${_f}.h")
            elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/${_f}.h")
                set (_header "${CMAKE_CURRENT_SOURCE_DIR}/src/${_f}.h")
            else()
        
            endif()
        
            ##################################################################
            # assume the LinkDef.h is under src
            ##################################################################

            set (_linkdef "${CMAKE_CURRENT_SOURCE_DIR}/src/${_f}LinkDef.h")


            ##################################################################
            # generate the ROOT dictionary
            ##################################################################
            ROOT_GENERATE_DICTIONARY(${CMAKE_CURRENT_BINARY_DIR}/src/${_f}Dict
                ${_header}
                LIBRARY ${_package}
                LINKDEF ${_linkdef}
                DEPENDS ${_header} ${_linkdef}
                        ${_xod_targets}
            )
        
            ##################################################################
            # the Dict/rootmap/pcm files generated from the ROOT dictionary
            ##################################################################
            list(APPEND src_${_package}_lists "${CMAKE_CURRENT_BINARY_DIR}/src/${_f}Dict.cxx")
            list(APPEND rootmap_${package}_list "${CMAKE_CURRENT_BINARY_DIR}/src/${_f}Dict.rootmap")
            list(APPEND pcm_${package}_list "${CMAKE_CURRENT_BINARY_DIR}/src/${_f}Dict_rdict.pcm")
        endforeach()
        
    endif(ARG_DICTS)


    ##########################################################################
    # = Build targets =
    ##########################################################################
    if (ARG_APPONLY)
        ######################################################################
        # = Build Application only =
        #
        # In this case, only one executable will be created.
        ######################################################################
        add_executable(${ARG_APPONLY}
            ${src_${_package}_lists}
            ${ARG_FILES}
        )

        target_link_libraries(${ARG_APPONLY}
            # ${ARG_DEPENDS}
            ${_depends_${package}_lists}
            ${ROOT_LIBRARIES}
            ${CLHEP_LIBRARIES}
        )

        ############# The App will be installed ################
        list(APPEND _package_install_bin_targets ${ARG_APPONLY})
        ########################################################
    elseif(ARG_PKGONLY)
        ######################################################################
        # = copy share only =
        #
        # In this case, the package don't create any targets. 
        ######################################################################
        if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/${_package})
            add_custom_target(${_package} ALL
              COMMAND ${CMAKE_COMMAND} -E make_directory
              ${CMAKE_BINARY_DIR}/share/${_package}
              COMMAND ${CMAKE_COMMAND} -E copy_directory
              ${CMAKE_CURRENT_SOURCE_DIR}/share
              ${CMAKE_BINARY_DIR}/share/${_package}
              COMMENT "Install share of ${_package}"
              )
        endif()
    else()
        
        ######################################################################
        # = Build App =
        ######################################################################
        #
        # In addition to the built libraries, an executable will be built. 
        #
        # ARG_FILES will be used to build the App when ARG_APP exists
        # 
        # The limitation is that only one app could be built in one package
        # 
        ######################################################################
        if (ARG_APP)
            add_executable(${ARG_APP}
                ${ARG_FILES}
            )

            target_link_libraries(${ARG_APP}
                ${_package}
            )

            ############# The App will be installed ############
            list(APPEND _package_install_bin_targets ${ARG_APP})
            ####################################################

        else()
            set(src_${_package}_lists ${src_${_package}_lists} ${ARG_FILES})
        endif()

        ######################################################################
        # = Build Library =
        ######################################################################
        if (src_${_package}_lists)
          ####################################################################
          # == Create the Libraies ==
          ####################################################################
          if (ARGS_MODULE)
              add_library(${_package} MODULE
                ${src_${_package}_lists}
                ${_other_srcs}
                )
          else()
              add_library(${_package} SHARED
                ${src_${_package}_lists}
                ${_other_srcs}
                )
          endif(ARGS_MODULE)

          target_link_libraries(${_package}
            # ${ARG_DEPENDS}
            ${_depends_${package}_lists}

            SNiPER::SniperKernel
            ${ROOT_LIBRARIES}
            )

          # = Decalre include directory =
          target_include_directories(${_package}
            PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            PUBLIC
                $<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}>
            PRIVATE
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
            )
          
          #######################################################
          ############# The Lib will be installed ###############
          # message("add shlib targets: ${_package}")
          list(APPEND _package_install_shlib_targets ${_package})
          #######################################################

          #######################################################
          ######## The public headers will be installed #########
          if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${_package})
              list(APPEND _package_install_include_dirs ${CMAKE_CURRENT_SOURCE_DIR}/${_package})
          endif()
          if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${_package})
              list(APPEND _package_install_include_dirs ${CMAKE_CURRENT_SOURCE_DIR}/include/${_package})
          endif()
          # message("include dirs of ${_package}: ${_package_install_include_dirs}")
          #######################################################



          # ============================================================================
          # ---- copy a merged rootmap/pcm under lib (BEGIN) ---------------------------
          # ============================================================================
  
          if (ARG_DICTS)
              add_custom_command(TARGET ${_package}
                                 POST_BUILD
                                 COMMAND cat ${rootmap_${package}_list} > ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib${_package}.rootmap
                                 COMMAND ${CMAKE_COMMAND} -E copy_if_different ${pcm_${package}_list} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
                                 VERBATIM
              )
          endif()
          
          # ============================================================================
          # ---- generate a merged rootmap under lib (END) -----------------------------
          # ============================================================================
        

        else()
          ####################################################################
          # == Create Interface Libraries ==
          #
          # The interface library means there is no library is generated,
          # but only provide the header files. 
          #
          # Here the assumption is that there is no source code under src. 
          ####################################################################
          add_library(${_package} INTERFACE)
          target_include_directories(${_package}
            INTERFACE
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            INTERFACE
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            INTERFACE
                $<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}>
            )

          #######################################################
          ############# The Lib will be installed ###############
          # message("add shlib targets: ${_package}")
          list(APPEND _package_install_shlib_targets ${_package})
          #######################################################

          #######################################################
          ######## The public headers will be installed #########
          if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${_package})
              list(APPEND _package_install_include_dirs ${CMAKE_CURRENT_SOURCE_DIR}/${_package})
          endif()
          if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${_package})
              list(APPEND _package_install_include_dirs ${CMAKE_CURRENT_SOURCE_DIR}/include/${_package})
          endif()
          # message("include dirs of ${_package}: ${_package_install_include_dirs}")
          #######################################################

        endif()


        # # = copy public header =
        # if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${_package})
        #     execute_process(
        #         COMMAND ${CMAKE_COMMAND} -E make_directory
        #             ${CMAKE_BINARY_DIR}/include/${_package}
        #         COMMAND ${CMAKE_COMMAND} -E copy_directory
        #             ${CMAKE_CURRENT_SOURCE_DIR}/${_package}
        #             ${CMAKE_BINARY_DIR}/include/${_package}
        #     )
        # endif()

        # = copy python at cmake configure stage =
        if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python/${_package})
            execute_process(
                COMMAND ${CMAKE_COMMAND} -E make_directory
                    ${CMAKE_BINARY_DIR}/python/${_package}
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/python/${_package}
                    ${CMAKE_BINARY_DIR}/python/${_package}
            )
        endif()
        #######################################################
        ############ The Python will be installed #############
        if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python/${_package})
            list(APPEND _package_install_python_dirs ${CMAKE_CURRENT_SOURCE_DIR}/python/${_package})
        endif()
        #######################################################


    endif()


    ##########################################################################
    # == at final, export these install targets/dirs ==
    ##########################################################################
    set_property(GLOBAL
        PROPERTY "${_package}InstallBinTargets" "${_package_install_bin_targets}"
    )
    set_property(GLOBAL
        PROPERTY "${_package}InstallShlibTargets" "${_package_install_shlib_targets}"
    )
    set_property(GLOBAL
        PROPERTY "${_package}InstallPythonDirs" "${_package_install_python_dirs}"
    )
    set_property(GLOBAL
        PROPERTY "${_package}InstallIncludeDirs" "${_package_install_include_dirs}"
    )

    # === targets ===
    set(_p_targets)
    foreach (_pt ${_package_install_bin_targets})
        if (TARGET ${_pt})
            list(APPEND _p_targets ${_pt})
        endif()
    endforeach()

    foreach (_pt ${_package_install_shlib_targets})
        if (TARGET ${_pt})
            list(APPEND _p_targets ${_pt})
        endif()
    endforeach()

    if (_p_targets)
        # a package name is also a target name
        install(TARGETS ${_p_targets}
                EXPORT ${PROJECT_NAME}Targets
                RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
                LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib)
    else()
        # the package name is not a target name. For example the APP
        message("Package ${_package}: no target found. ")
    endif()

    # === dirs ===
    # ==== headers, including the interfaces ====
    if (_package_install_include_dirs)
        foreach(_pt ${_package_install_include_dirs})
            install(DIRECTORY ${_pt}
                    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
        endforeach()
    endif()
    # ==== rootmap, dict etc. in the lib ====
    # if (EXISTS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib${_package}.rootmap)
    # endif()
    if (pcm_${package}_list)
        install(FILES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib${_package}.rootmap
                DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib)
        install(FILES ${pcm_${package}_list}
                DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib)
    endif()

    # ==== python ====
    if (_package_install_python_dirs)
        foreach(_pt ${_package_install_python_dirs})
            install(DIRECTORY ${_pt}
                    DESTINATION python)
        endforeach()
    endif()

endfunction(PKG)
