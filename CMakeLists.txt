cmake_minimum_required(VERSION 3.13)
project(AoSSimulator)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 gtk+-3.0 gtkmm-3.0)
if(GTK3_FOUND)
    include_directories(${GTK3_INCLUDE_DIRS})
    link_directories(${GTK3_LIBRARY_DIRS})
endif()

set(CMAKE_CXX_STANDARD 20)

if(NOT EMSCRIPTEN)
    add_executable(
            AoSManoAMano
            app/main.cpp
            include/AgeOfSigmarSim.h)

    target_link_libraries(
            AoSManoAMano
            AoSSim
    )
    target_include_directories(
            AoSManoAMano PRIVATE
            include
    )
endif()

add_subdirectory("src")

if(NOT EMSCRIPTEN)
    add_subdirectory("test")
    add_subdirectory("gtkManoAMano")
    add_subdirectory("queryTool")
else()
    add_subdirectory("web")
endif()