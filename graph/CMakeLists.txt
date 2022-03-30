cmake_minimum_required (VERSION 3.10)

project (PozhiloyPumba_Graph VERSION 1.0)

find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/sources/*.[ch]pp
        ${PROJECT_SOURCE_DIR}/sources/*.[ch]
        ${PROJECT_SOURCE_DIR}/include/*.[h]pp
        ${PROJECT_SOURCE_DIR}/include/*.[h]
    )

    add_custom_target(
        format
        COMMAND clang-format
        -i
        -style=file
        ${ALL_CXX_SOURCE_FILES}
    )
endif()

set (EXEC_FILE "graph")

set (SOURCES
    sources/main.cpp
)

set (HEADERS
    include/graph.hpp
)

add_executable(${EXEC_FILE} ${SOURCES})

target_include_directories(${EXEC_FILE} PRIVATE include)

set (COMPILER_SETTINGS
    -std=c++2a
    -Wall
    -Wextra
    -Wpedantic

    -Wsign-promo
    -Wdouble-promotion
    
    -Wfloat-equal

    -Wdiv-by-zero
    
    -O2
)

target_compile_options(${EXEC_FILE} PRIVATE ${COMPILER_SETTINGS})