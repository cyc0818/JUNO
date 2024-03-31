
# Global Options
## Python 

find_package(Python COMPONENTS Interpreter Development)
if (Python_FOUND)
  message("Found Python ${Python_LIBRARIES}")
endif()
include_directories(${Python_INCLUDE_DIRS})
link_directories($ENV{JUNO_EXTLIB_Python_HOME}/lib)

## Boost
find_package(Boost REQUIRED COMPONENTS python filesystem system regex)
if (Boost_FOUND)
  message("Found Boost: ${Boost_LIBRARIES}")
endif()
include_directories($ENV{JUNO_EXTLIB_Boost_HOME})
link_directories($ENV{JUNO_EXTLIB_Boost_HOME}/lib)

## XercesC
find_package(XercesC REQUIRED)
if(XercesC_FOUND)
  message("Found XercesC: ${XercesC_INCLUDE_DIRS} ${XercesC_LIBRARIES}")
  if (TARGET XercesC::XercesC)
    message("The XercesC found target: XercesC::XercesC")
  endif()
endif()

## Log4cpp
include_directories($ENV{JUNO_EXTLIB_log4cpp_HOME}/include)
find_library(LOG4CPP_LIBRARIES log4cpp
  HINTS $ENV{JUNO_EXTLIB_log4cpp_HOME}/lib)

## XML2
find_package(libxml2)

## GSL
find_package(GSL)

## ROOT
include_directories($ENV{JUNO_EXTLIB_ROOT_HOME}/include)
find_program(ROOT_CONFIG_EXECUTABLE root-config
    PATHS $ENV{JUNO_EXTLIB_ROOT_HOME}/bin)
# execute_process(
#     COMMAND ${ROOT_CONFIG_EXECUTABLE} --evelibs
#     OUTPUT_VARIABLE ROOT_LIBRARIES
#     OUTPUT_STRIP_TRAILING_WHITESPACE
# )

find_package(ROOT 
             COMPONENTS Core EG Eve 
                        Geom Graf Graf3d 
                        MathCore MathMore Net 
                        RIO Tree TreePlayer
             REQUIRED)
message("Found ROOT: ${ROOT_LIBRARIES}")

link_directories($ENV{JUNO_EXTLIB_ROOT_HOME}/lib)

## CLHEP
include_directories($ENV{JUNO_EXTLIB_CLHEP_HOME}/include)
find_library(CLHEP_LIBRARIES CLHEP 
    HINTS $ENV{JUNO_EXTLIB_CLHEP_HOME}/lib
)
link_directories($ENV{JUNO_EXTLIB_CLHEP_HOME}/lib)

## Geant4
find_package(Geant4 REQUIRED ui_all vis_all)
include_directories($ENV{JUNO_EXTLIB_Geant4_HOME}/include/geant4)
find_program(GEANT4_CONFIG_EXECUTABLE geant4-config
    PATHS $ENV{JUNO_EXTLIB_Geant4_HOME}/bin)
execute_process(
    COMMAND ${GEANT4_CONFIG_EXECUTABLE} --cflags
    OUTPUT_VARIABLE GEANT4_CFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
# remove c++11 from command line
string(REPLACE "-std=c++11" "" GEANT4_CFLAGS ${GEANT4_CFLAGS})
message("testing GEANT4_CFLAGS... ${GEANT4_CFLAGS}")
add_definitions(${GEANT4_CFLAGS})

execute_process(
    COMMAND ${GEANT4_CONFIG_EXECUTABLE} --libs
    OUTPUT_VARIABLE GEANT4_LIBRARIES
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
link_directories($ENV{JUNO_EXTLIB_Geant4_HOME}/lib)


if (DEFINED ENV{JUNO_EXTLIB_custom4_HOME})
    set(Custom4_VERBOSE ON)
    find_package(Custom4 REQUIRED) 
    #[=[
    message(STATUS "Custom4_FOUND         : ${Custom4_FOUND} ")
    message(STATUS "Custom4_VERSION       : ${Custom4_VERSION} ")
    message(STATUS "Custom4_PREFIX        : ${Custom4_PREFIX} ")
    message(STATUS "Custom4_INCLUDE_DIR   : ${Custom4_INCLUDE_DIR} ")
    message(STATUS "Custom4_INCLUDE_DIRS  : ${Custom4_INCLUDE_DIRS} ")
    message(STATUS "Custom4_CFLAGS        : ${Custom4_CFLAGS} ")
    message(STATUS "Custom4_DEFINITIONS   : ${Custom4_DEFINITIONS} ")
    message(STATUS "Custom4_LIBRARY_DIR   : ${Custom4_LIBRARY_DIR} ")
    message(STATUS "Custom4_LIBRARIES     : ${Custom4_LIBRARIES} ")
    message(STATUS "Custom4_CMAKE_PATH    : ${Custom4_CMAKE_PATH} ")
    message(STATUS "Custom4_CMAKE_DIR     : ${Custom4_CMAKE_DIR} ")
    #]=]

    set(CUSTOM4_LIBRARIES   ${Custom4_LIBRARIES})     # -LDIR -lCustom4 for linking 
    set(CUSTOM4_DEFINITIONS ${Custom4_DEFINITIONS})   # -DWITH_CUSTOM4 for compiling
    set(CUSTOM4_CFLAGS      ${Custom4_CFLAGS})        # -IINCDIR for compiling 

    message(STATUS "CUSTOM4_LIBRARIES   : ${CUSTOM4_LIBRARIES}")
    message(STATUS "CUSTOM4_DEFINITIONS : ${CUSTOM4_DEFINITIONS}")
    message(STATUS "CUSTOM4_CFLAGS      : ${CUSTOM4_CFLAGS}")

    add_definitions(${CUSTOM4_DEFINITIONS})
    add_definitions(${CUSTOM4_CFLAGS})
endif()


## HepMC
include_directories($ENV{JUNO_EXTLIB_HepMC_HOME}/include)
find_library(HEPMC_LIBRARIES HepMC 
    HINTS $ENV{JUNO_EXTLIB_HepMC_HOME}/lib
)
link_directories($ENV{JUNO_EXTLIB_HepMC_HOME}/lib)

## GENIE
include_directories($ENV{JUNO_EXTLIB_genie_HOME}/include/GENIE)
find_program(GENIE_CONFIG_EXECUTABLE genie-config
    PATHS $ENV{JUNO_EXTLIB_genie_HOME}/bin)
execute_process(
    COMMAND ${GENIE_CONFIG_EXECUTABLE} --libs
    OUTPUT_VARIABLE GENIE_LIBRARIES
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

## frontier
include_directories($ENV{JUNO_EXTLIB_frontier_HOME}/include)
find_library(FRONTIER_LIBRARIES frontier_client
    HINTS $ENV{JUNO_EXTLIB_frontier_HOME}/lib)
link_directories($ENV{JUNO_EXTLIB_frontier_HOME}/lib)

## curl
find_package(CURL QUIET)
include_directories(${CURL_INCLUDE_DIRS})

## mysql/mysql connector c++
include_directories($ENV{JUNO_EXTLIB_mysql_connector_c_HOME}/include)
find_library(MYSQL_CONNECTOR_C_LIBRARIES mysqlclient
    HINTS $ENV{JUNO_EXTLIB_mysql_connector_c_HOME}/lib)
include_directories($ENV{JUNO_EXTLIB_mysql_connector_cpp_HOME}/include)
find_library(MYSQL_CONNECTOR_CPP_LIBRARIES mysqlcppconn
    HINTS $ENV{JUNO_EXTLIB_mysql_connector_cpp_HOME}/lib)

## sqlite
find_package(SQLite3)

## yaml
include_directories($ENV{JUNO_EXTLIB_libyaml_HOME}/include)
find_library(YAML_LIBRARIES yaml
    HINTS $ENV{JUNO_EXTLIB_libyaml_HOME}/lib)

## Sniper
find_package(SNiPER)
add_definitions(-DSNIPER_VERSION_2)

## Opticks
if(DEFINED ENV{OPTICKS_PREFIX})
   set(Opticks_VERBOSE YES)
   set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "$ENV{JUNOTOP}/opticks/cmake/Modules")
   find_package(Opticks MODULE)
   message(STATUS "${CMAKE_CURRENT_LIST_FILE} : Opticks_FOUND:${Opticks_FOUND}" )
endif()

## onnx runtime
if (DEFINED ENV{JUNO_EXTLIB_libonnxruntime_HOME})
    find_library(ONNXRUNTIME_LIBRARIES onnxruntime
                 HINTS $ENV{JUNO_EXTLIB_libonnxruntime_HOME}/lib64)
    include_directories($ENV{JUNO_EXTLIB_libonnxruntime_HOME}/include/onnxruntime)
    set(ONNXRUNTIME_FOUND TRUE)
    message("Found libonnxruntime ${ONNXRUNTIME_LIBRARIES}")
endif()

## NuWro Interface
include_directories($ENV{JUNO_EXTLIB_nuwro_HOME}/include)
find_library(NUWRO_LIBRARIES nuwro_interface 
    HINTS $ENV{JUNO_EXTLIB_nuwro_HOME}/lib
)
# define NuWro_Found if NuWro_LIBRARIES is not empty
if(NUWRO_LIBRARIES)
    set(NuWro_Found TRUE)
else()
    set(NuWro_Found FALSE)
endif()
link_directories($ENV{JUNO_EXTLIB_nuwro_HOME}/lib)