# Download and unpack nanogui at configure time
configure_file(CMakeLists.txt.in nanogui-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/nanogui-download )
if(result)
    message(FATAL_ERROR "CMake step for nanogui failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/nanogui-download )
if(result)
    message(FATAL_ERROR "Build step for nanogui failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add nanogui directly to our build. This defines
# the nanogui target.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/nanogui-src
        ${CMAKE_CURRENT_BINARY_DIR}/nanogui-build
        EXCLUDE_FROM_ALL)

# The nanogui targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${nanogui_SOURCE_DIR}/include")
endif()

add_executable(
        AoSManoAManoGUI
        guimain.cpp
)

target_include_directories(
        AoSManoAManoGUI PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/nanogui-src/ext/eigen
        ${CMAKE_CURRENT_BINARY_DIR}/nanogui-src/ext/nanovg/src
        ${CMAKE_CURRENT_BINARY_DIR}/nanogui-src/include
        include
)

target_link_libraries(
        AoSManoAManoGUI
        whSim
        nanogui
)
