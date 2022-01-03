cmake_minimum_required (VERSION 3.0)

project (PozhiloyPumba_detMatrix VERSION 1.0)

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

set (EXEC_FILE "matrix")
set (TEST_FILE "tests")

set (GTEST_ROOT ${CMAKE_SOURCE_DIR}/googletests/gTest)
find_package(GTest REQUIRED)

set (TESTS	
    sources/test.cpp
)

set (SOURCES
    sources/main.cpp
)

set (HEADERS
    include/matrix.hpp
)

add_executable(${EXEC_FILE} ${SOURCES})
add_executable(${TEST_FILE} ${TESTS})

target_include_directories(${EXEC_FILE} PRIVATE include)
target_include_directories(${TEST_FILE} PRIVATE include)

target_link_libraries(${TEST_FILE} PRIVATE "${GTEST_LIBRARIES}" "pthread")
target_include_directories(${TEST_FILE} PRIVATE headers "${GTEST_INCLUDE_DIRS}")

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic

    -Wsign-promo
    -Wsign-conversion
    -Wdouble-promotion

    -Wfloat-equal
    -Wbool-compire

    -Wdiv-by-zero

    -O2
)

set (COMPILER_SETTINGS
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