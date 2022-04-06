cmake_minimum_required (VERSION 3.10)

project (PozhiloyPumba_BitonicSort VERSION 1.0)

# clang-format
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

# if OpenCL doesn't install it is not worked
find_package(OpenCL REQUIRED)

# compiler options
if(MSVC)
    add_compile_options("/W4" "$<$<CONFIG:RELEASE>:/O2>")
else()
    add_compile_options("-Wall" "-Wextra" "$<$<CONFIG:RELEASE>:-O2>")
endif()

# required standart
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# enable test system
enable_testing()

add_subdirectory(tests)

# adding custom targets
add_custom_target(
    check_correct
    COMMAND ${CMAKE_CTEST_COMMAND} -L CORRECT_TESTS
)

add_custom_target(
    check_perf
    COMMAND ${CMAKE_CTEST_COMMAND} -V -L PERF_TESTS
)

set (EXEC_FILE "sort")
set (GEN_FILE "generator")

set (SOURCES
    sources/main.cpp
)

set (GEN
    sources/generator.cpp
)

add_executable(${GEN_FILE} ${GEN})
add_executable(${EXEC_FILE} ${SOURCES})

target_include_directories(${EXEC_FILE} PRIVATE include)

# linking with OpenCL
target_link_libraries(${EXEC_FILE} OpenCL::OpenCL)

if(NVIDIA)
    target_compile_definitions(${EXEC_FILE} PRIVATE NVIDIA)
endif()

if(KERNEL_SOURCE)
    target_compile_definitions(${EXEC_FILE} PRIVATE KERNEL_SOURCE="${KERNEL_SOURCE}")
endif()
