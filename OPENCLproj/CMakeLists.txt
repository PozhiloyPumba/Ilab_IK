cmake_minimum_required (VERSION 3.20)

project (PozhiloyPumba_BitonicSort VERSION 1.0)

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

find_package(OpenCL REQUIRED)

set (EXEC_FILE "sort")

set (SOURCES
    sources/main.cpp
    sources/app.cpp
)

set (HEADERS
    include/app.hpp
)

add_executable(${EXEC_FILE} ${SOURCES})
target_include_directories(${EXEC_FILE} PRIVATE include)

target_link_libraries(${EXEC_FILE} OpenCL::OpenCL)

set (COMPILER_SETTINGS
    -std=c++2a
    -Wall
    -Wextra
    -Wpedantic

    -Wsign-promo
    -Wsign-conversion
    -Wdouble-promotion

    -Wfloat-equal
    -Wbool-compare

    -Wdiv-by-zero

    -O2
)

target_compile_options(${EXEC_FILE} PRIVATE ${COMPILER_SETTINGS})