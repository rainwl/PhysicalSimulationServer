cmake_minimum_required(VERSION 3.26)
project(
        PhysicalSimulationDriver
        LANGUAGES CXX
        DESCRIPTION "send simulated data to physics engine"
        VERSION 0.0.1
)

set(CMAKE_CXX_STANDARD 23)

add_executable(PhysicalSimulationDriver
        src/main.cpp
        protobuf/coord.pb.cc
        protobuf/coord.pb.h
        protobuf/fusion.pb.cc
        protobuf/fusion.pb.h
        protobuf/haptic.pb.cc
        protobuf/haptic.pb.h
        protobuf/offset.pb.cc
        protobuf/offset.pb.h
        protobuf/tissue.pb.cc
        protobuf/tissue.pb.h
)

target_include_directories(PhysicalSimulationDriver PRIVATE include)
target_include_directories(PhysicalSimulationDriver PRIVATE protobuf)

find_library(ECAL_CORE NAMES ecal_core PATHS library)
find_library(ECAL_PROTO NAMES ecal_proto PATHS library)
find_library(LIBPROTOBUF NAMES libprotobuf PATHS library)

if (ECAL_CORE AND ECAL_PROTO AND LIBPROTOBUF)
    target_link_libraries(PhysicalSimulationDriver
            ${ECAL_LIB}
            ${ECAL_PROTO}
            ${LIBPROTOBUF}
    )
else ()
    message(FATAL_ERROR "PSD can not find eCAL lib")
endif ()

