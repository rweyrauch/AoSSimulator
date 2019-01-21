cmake_minimum_required(VERSION 3.10)
project(WarhammerSim)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

find_package(GTK2 REQUIRED gtk gtkmm)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        WarhammerSim
        app/main.cpp
        include/WarhammerSim.h)

target_link_libraries(
        WarhammerSim
        whSim
        ${JSONCPP_LIBRARIES}
)

target_include_directories(
        WarhammerSim PRIVATE
        include
)

add_subdirectory("src")
add_subdirectory("test")