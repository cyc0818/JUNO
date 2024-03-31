
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

## XercesC
find_package(XercesC REQUIRED)
if(XercesC_FOUND)
  message("Found XercesC: ${XercesC_INCLUDE_DIRS} ${XercesC_LIBRARIES}")
  if (TARGET XercesC::XercesC)
    message("The XercesC found target: XercesC::XercesC")
  endif()
endif()

## ROOT
include_directories($ENV{JUNO_EXTLIB_ROOT_HOME}/include)
find_program(ROOT_CONFIG_EXECUTABLE root-config
    PATHS $ENV{JUNO_EXTLIB_ROOT_HOME}/bin)

find_package(ROOT 
             COMPONENTS Core EG Eve 
                        Geom Graf Graf3d 
                        MathCore Net 
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

## HepMC
include_directories($ENV{JUNO_EXTLIB_HepMC_HOME}/include)
find_library(HEPMC_LIBRARIES HepMC 
    HINTS $ENV{JUNO_EXTLIB_HepMC_HOME}/lib
)
link_directories($ENV{JUNO_EXTLIB_HepMC_HOME}/lib)

## frontier
include_directories($ENV{JUNO_EXTLIB_frontier_HOME}/include)
find_library(FRONTIER_LIBRARIES frontier_client
    HINTS $ENV{JUNO_EXTLIB_frontier_HOME}/lib)
link_directories($ENV{JUNO_EXTLIB_frontier_HOME}/lib)

## sqlite
find_package(SQLite3)

## Sniper
find_package(SNiPER)
add_definitions(-DSNIPER_VERSION_2)


