
##############################################################################
# = Define options for JUNO offline =
#
#   Following rules are used in cmake:
#   - The command 'option' is used to define an OFF/ON option
#   - The command 'set' is used to define the other options
#   All these options can be overridden in the command line by -DKEY=VALUE
#
##############################################################################

##############################################################################
# == OEC related  ==
##############################################################################
option (WITH_OEC "Enable OEC in offline" ON)

if (WITH_OEC)
    message("OEC is enabled in offline")
    add_definitions(-DWITH_OEC)
endif()

#When built for online, JUNO_ONLINE should be turned on
option (BUILD_ONLINE "BUILD_ONLINE" OFF)

if (BUILD_ONLINE)
    message("It will be built for online")
    add_definitions(-DBUILD_ONLINE)
endif()


# Conditional compilation with the preprocessors for PERFORMANCE_CHECK
option (PERFORMANCE_CHECK "Enable saving time and memory consumption variables for some algorithms" OFF)

if (PERFORMANCE_CHECK)
    message("Saving time and memory consumption variables for some algorithms is enabled")
    add_definitions(-DPERFORMANCE_CHECK)
endif()
