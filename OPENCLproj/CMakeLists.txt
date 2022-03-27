cmake_minimum_required (VERSION 3.22)

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
set (GEN_FILE "generator")
set (TEST_FILE "tests")

find_package(GTest REQUIRED)

set (SOURCES
    sources/main.cpp
)

set (GEN
    sources/generator.cpp
)

set (TESTS	
    sources/testing.cpp
)

set (HEADERS
    include/app.hpp
)

add_executable(${TEST_FILE} ${TESTS})
add_executable(${GEN_FILE} ${GEN})
add_executable(${EXEC_FILE} ${SOURCES})

target_include_directories(${EXEC_FILE} PRIVATE include)
target_include_directories(${TEST_FILE} PRIVATE include)
target_link_libraries(${TEST_FILE} PRIVATE "${GTEST_LIBRARIES}" "pthread" OpenCL::OpenCL)

target_link_libraries(${EXEC_FILE} OpenCL::OpenCL)

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
    
if(TIMER)
    target_compile_definitions(${EXEC_FILE} PRIVATE TIMER)
endif()

if(NVIDIA)
    target_compile_definitions(${EXEC_FILE} PRIVATE NVIDIA)
endif()

target_compile_options(${EXEC_FILE} PRIVATE ${COMPILER_SETTINGS})
target_compile_options(${GEN_FILE} PRIVATE ${COMPILER_SETTINGS})
target_compile_options(${TEST_FILE} PRIVATE ${COMPILER_SETTINGS})