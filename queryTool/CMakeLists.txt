
add_executable(
        aosQueryTool
        main.cpp
        cxxopts.hpp
)

target_include_directories(
        aosQueryTool PUBLIC
        ../include
)

target_link_libraries(
        aosQueryTool
        AoSSim
)