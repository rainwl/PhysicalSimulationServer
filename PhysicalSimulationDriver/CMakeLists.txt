cmake_minimum_required(VERSION 3.26)
project(
        PhysicalSimulationDriver
        LANGUAGES CXX
        DESCRIPTION "send simulated data to physics engine"
        VERSION 0.0.1
)

set(CMAKE_CXX_STANDARD 23)

add_executable(PhysicalSimulationDriver include/main.cpp)

target_include_directories(PhysicalSimulationDriver PRIVATE include)
