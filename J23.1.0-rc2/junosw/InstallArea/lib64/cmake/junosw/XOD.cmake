### INCLUDE GUARD BEGIN ###
if(__offline_XOD_macro)
    return()
endif()
set(__offline_XOD_macro true)
### INCLUDE GUARD END ###

# XOD still uses Python2
find_package(Python2 QUIET COMPONENTS Interpreter)
if (NOT Python2_FOUND)
  set(Python2_EXECUTABLE "/usr/bin/python2")
endif()

# handle XOD outside offline
set(XOD_PREFIX)

if (DEFINED ENV{XMLOBJDESCROOT})
    set(XOD_PREFIX $ENV{XMLOBJDESCROOT})
else()
    set(XOD_PREFIX ${CMAKE_SOURCE_DIR}/XmlObjDesc)
endif()

message("XOD_PREFIX: ${XOD_PREFIX}")

##############################################################################
# Implement the XOD based on the execute_process and configure (BEGIN)
##############################################################################

function(xod_gen_lists _label)
    CMAKE_PARSE_ARGUMENTS(ARG "" "" "FILES;SRCS" "" ${ARGN})
    set(_files "${ARG_FILES}")

    ##########################################################################
    # xdd.dtd
    ##########################################################################
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${XOD_PREFIX}/xml_files/xdd.dtd
            ${CMAKE_CURRENT_BINARY_DIR}/xml/xdd.dtd
    )

    ##########################################################################
    # xml -> header -> impl (optional)
    ##########################################################################


    foreach (_file ${_files})
        ##########################################################################
        # xml -> header
        ##########################################################################
        if (${CMAKE_CURRENT_SOURCE_DIR}/xml/${_file}.xml
                IS_NEWER_THAN
                ${CMAKE_CURRENT_BINARY_DIR}/Event/${_file}.obj2doth)
            #####################################################################
            # COPY XML to BINARY 
            #####################################################################

            execute_process(
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            ${CMAKE_CURRENT_SOURCE_DIR}/xml/${_file}.xml
                            ${CMAKE_CURRENT_BINARY_DIR}/xml/${_file}.xml
            )

            #####################################################################
            # GENERATE header file and link defs
            #   - ${CMAKE_CURRENT_BINARY_DIR}/Event/${_file}.h
            #   - ${CMAKE_CURRENT_BINARY_DIR}/src/${_file}LinkDef.h
            #   - ${CMAKE_CURRENT_BINARY_DIR}/Event/${_file}.obj2doth
            #####################################################################
    
            execute_process(
                COMMAND ${Python2_EXECUTABLE} ${XOD_PREFIX}/scripts/godII.py 
                            -n JM
                            -f -b ${CMAKE_CURRENT_BINARY_DIR}/src 
                            -g src -r ${XOD_PREFIX} 
                                ${CMAKE_CURRENT_BINARY_DIR}/xml/${_file}.xml
                # COMMAND_ECHO STDERR
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Event
            )
    
            execute_process(
                COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/Event/${_file}.obj2doth
                # COMMAND_ECHO STDERR
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Event
            )

            list(APPEND xod_generated_${_label}_list
                        ${CMAKE_CURRENT_BINARY_DIR}/Event/${_file}.h
                        ${CMAKE_CURRENT_BINARY_DIR}/src/${_file}LinkDef.h
            )

        endif()
    

        ##########################################################################
        # header -> impl (optional)
        ##########################################################################

        set(_index -1)
        if (ARG_SRCS)
            list (FIND ARG_SRCS "${_file}" _index)
        endif()
    
        # if the src/*.cc already exists in the source, skip the generation
        # Two cases:
        # - user specify it in the SRCS
        # - the file is in SOURCE
        if (${_index} GREATER -1)

        elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/${_file}.cc)
    
        else()
            #####################################################################
            # GENERATE impl file
            #   - ${CMAKE_CURRENT_BINARY_DIR}/src/${_file}.cc
            #####################################################################
            if (${CMAKE_CURRENT_SOURCE_DIR}/xml/${_file}.xml
                    IS_NEWER_THAN
                    ${CMAKE_CURRENT_BINARY_DIR}/src/${_file}.cc)
                execute_process(
                COMMAND ${Python2_EXECUTABLE} ${XOD_PREFIX}/scripts/genSrc.py 
                            ${CMAKE_CURRENT_BINARY_DIR}/Event/${_file}.h
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src
                # COMMAND_ECHO STDERR
            )
            list(APPEND xod_generated_${_label}_list ${CMAKE_CURRENT_BINARY_DIR}/src/${_file}.cc)
        endif()
        endif()
    endforeach(_file)


endfunction()

