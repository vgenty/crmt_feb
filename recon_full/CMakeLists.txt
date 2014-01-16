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

## -- setup to compile libraries
add_library(base SHARED src/Fiber.cxx src/Track.cxx src/Geometry.cxx src/Line.cxx)
add_library(particle SHARED src/Detector.cxx src/FileManager.cxx src/RecoModule.cxx )

## -- add library dependencies
target_link_libraries(base ${ROOT_LIBS})
target_link_libraries(particle ${ROOT_LIBS} base)


##### COMPILE Recon EXECUTABLE
## -- setup to compile executable 
add_executable(Recon src/test_reco.cxx)
## -- add executable dependencies
target_link_libraries(Recon ${ROOT_LIBS} particle base)
