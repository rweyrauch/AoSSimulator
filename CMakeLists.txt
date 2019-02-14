cmake_minimum_required(VERSION 3.10)
project(AoSSimulator)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0 gtkmm-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

set(CMAKE_CXX_STANDARD 14)

add_executable(
        AoSManoAMano
        app/main.cpp
        include/WarhammerSim.h)

target_link_libraries(
        AoSManoAMano
        whSim
        ${JSONCPP_LIBRARIES}
)

target_include_directories(
        AoSManoAMano PRIVATE
        include
)

add_subdirectory("src")
add_subdirectory("test")
add_subdirectory("gtkManoAMano")
