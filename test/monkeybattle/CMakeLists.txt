
add_executable(
        monkeytest
        main.cpp
)

target_link_libraries(
        monkeytest
        whSim
        ${GTK2_LIBRARIES}
)

target_include_directories(
        monkeytest PRIVATE
        ../../include
)
