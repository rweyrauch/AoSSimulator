cmake_minimum_required(VERSION 3.13)
project(WarhammerSim)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        WarhammerSim
        app/main.cpp
)

target_link_libraries(
        WarhammerSim
        whSim
)

target_include_directories(
        WarhammerSim PRIVATE
        include
)

add_subdirectory("src")
add_subdirectory("test")