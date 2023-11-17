cmake_minimum_required(VERSION 3.26)
project(
        PhysicalSimulationDriver
        LANGUAGES CXX
        DESCRIPTION "send simulated data to physics engine"
        VERSION 0.0.1
)

set(CMAKE_CXX_STANDARD 23)

add_executable(PhysicalSimulationDriver src/main.cpp)

target_include_directories(PhysicalSimulationDriver PRIVATE include)
target_include_directories(PhysicalSimulationDriver PRIVATE protobuf)
#target_link_libraries(PhysicalSimulationDriver
#        library/ecal_core.lib
#        library/ecal_proto.lib
#        library/libprotobuf.lib)