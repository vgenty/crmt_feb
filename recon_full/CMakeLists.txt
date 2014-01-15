cmake_minimum_required(VERSION 2.8)
project(RECON)

## -- set some cmake variables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_CXX_FLAGS "-g -Wall -O3 -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})

## -- prepare ROOT
find_package(ROOT REQUIRED)
include_directories(${ROOT_INC_DIR})
link_directories(${ROOT_LIB_DIR})

## -- prepare project include and lib dirs
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

##### COMPILE CRMT LIBRARIES
## -- setup to compile libraries
#add_library(etc SHARED src/ParameterSpace.cxx src/FileManager.cxx src/Geometry.cxx)
#add_library(particle SHARED src/Track.cxx src/Fiber.cxx src/RecoModule.cxx src/ParameterSpace.cxx src/FileManager.cxx src/Geometry.cxx src/Detector.cxx src/Line.cxx)
#add_library(particle SHARED src/Track.cxx src/Fiber.cxx src/RecoModule.cxx src/FileManager.cxx src/Geometry.cxx src/Detector.cxx src/Line.cxx)
add_library(particle SHARED src/Track.cxx src/Fiber.cxx src/RecoModule.cxx src/Geometry.cxx src/Detector.cxx src/Line.cxx)
## -- add library dependencies
target_link_libraries(particle ${ROOT_LIBS})
#target_link_libraries(particle ${ROOT_LIBS} Geo)
#target_link_libraries(particle ${ROOT_LIBS})	

##### COMPILE CRMT EXECUTABLE
## -- setup to compile executable 
add_executable(Recon src/test_reco.cxx)
## -- add executable dependencies
target_link_libraries(Recon ${ROOT_LIBS} particle)
