cmake_minimum_required(VERSION 3.10)
project(AoSSimulator)

option(JsBuild "JsBuild" OFF)

if(NOT JsBuild)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0 gtkmm-3.0)
	include_directories(${GTK3_INCLUDE_DIRS})
	link_directories(${GTK3_LIBRARY_DIRS})
endif(NOT JsBuild)

set(CMAKE_CXX_STANDARD 14)

if(NOT JsBuild)
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
endif(NOT JsBuild)

#if(JsBuild)
#target_link_options(
#        AoSManoAMano PRIVATE
#        --bind
#)
#endif(JsBuild)

add_subdirectory("src")
if(NOT JsBuild)
        add_subdirectory("test")
        add_subdirectory("gtkManoAMano")
        add_subdirectory("queryTool")
endif(NOT JsBuild)
