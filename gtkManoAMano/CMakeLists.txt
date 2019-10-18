

add_executable(
        gtkManoAMano
        main.cpp
        ResultsDisplay.cpp
)

target_include_directories(
        gtkManoAMano PUBLIC
        ${GTK3_INCLUDE_DIRS}
        ../include
)

target_link_libraries(
        gtkManoAMano
        AoSSim
        ${GTK3_LIBRARIES}
)