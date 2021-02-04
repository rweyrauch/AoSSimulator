project(WarhammerSim)

set(AOSSIM_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/glue_stub.cpp)

add_executable(AoSSimWeb ${AOSSIM_SOURCE})

ADD_CUSTOM_COMMAND (
        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/aossim.idl
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/aossim_glue.js ${CMAKE_CURRENT_BINARY_DIR}/aossim_glue.cpp
        COMMAND python3 $ENV{EMSDK}/upstream/emscripten/tools/webidl_binder.py ${CMAKE_CURRENT_SOURCE_DIR}/aossim.idl ${CMAKE_CURRENT_BINARY_DIR}/aossim_glue
)
SET_PROPERTY(SOURCE glue_stub.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/aossim_glue.cpp)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1 -s MODULARIZE=1 -s NO_FILESYSTEM=0 -s INVOKE_RUN=0 -s ENVIRONMENT=web -s EXPORT_BINDINGS=1 -s EXPORT_NAME='${CMAKE_PROJECT_NAME}' -s RESERVED_FUNCTION_POINTERS=20 -s EXPORTED_RUNTIME_METHODS=[] --memory-init-file 0 -s NO_EXIT_RUNTIME=1 --post-js ${CMAKE_CURRENT_BINARY_DIR}/aossim_glue.js")

target_include_directories(
        AoSSimWeb PUBLIC
        ../include
        ${CMAKE_CURRENT_BINARY_DIR}
)
target_compile_options(
        AoSSimWeb PRIVATE
        -Wpedantic
        -Wextra
        -Wall
        -Wformat-security
        -Wno-unused-parameter
        -fno-exceptions
        -frtti
        -fbracket-depth=512
)
target_link_libraries(
        AoSSimWeb PRIVATE
        AoSSim
)

