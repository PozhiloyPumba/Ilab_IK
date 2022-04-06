set (PERF_TEST_SRC
    perfTesting.cpp
)

set (PERF_TEST_EXEC "perf_testing")

# perf test exec
add_executable(${PERF_TEST_EXEC} ${PERF_TEST_SRC})
target_include_directories(${PERF_TEST_EXEC} PRIVATE ../include)
target_link_libraries(${PERF_TEST_EXEC} OpenCL::OpenCL)
target_compile_definitions(${PERF_TEST_EXEC} PRIVATE KERNEL_SOURCE="../../sources/kernels/simple.cl")

if(NVIDIA)
    target_compile_definitions(${PERF_TEST_EXEC} PRIVATE NVIDIA)
endif()

# adding tests
add_test(NAME PerfTest1M COMMAND ${PERF_TEST_EXEC} ${CMAKE_CURRENT_SOURCE_DIR}/testdata/test3.dat)
add_test(NAME PerfTest2M COMMAND ${PERF_TEST_EXEC} ${CMAKE_CURRENT_SOURCE_DIR}/testdata/test4.dat)
add_test(NAME PerfTest4M COMMAND ${PERF_TEST_EXEC} ${CMAKE_CURRENT_SOURCE_DIR}/testdata/test5.dat)
add_test(NAME PerfTest8M COMMAND ${PERF_TEST_EXEC} ${CMAKE_CURRENT_SOURCE_DIR}/testdata/test6.dat)

SET_TESTS_PROPERTIES(PerfTest1M PerfTest2M PerfTest4M PerfTest8M PROPERTIES LABELS "PERF_TESTS")

# correct tests
set (CORRECT_TEST_SRC 
    correctTesting.cpp
)

# for template tests I need different binary files
set (F_TEST_EXEC "correct_float_test")
set (I_TEST_EXEC "correct_int_test")
set (U_TEST_EXEC "correct_unsigned_test")
set (SH_TEST_EXEC "correct_short_test")

# foreach file linking with OpenCL, targeting include directory...
list(APPEND CORRECT_TESTS_EXEC ${F_TEST_EXEC} ${I_TEST_EXEC} ${U_TEST_EXEC} ${SH_TEST_EXEC})

foreach(X IN LISTS CORRECT_TESTS_EXEC)
    add_executable(${X} ${CORRECT_TEST_SRC})
    target_include_directories(${X} PRIVATE ../include)
    target_link_libraries(${X} OpenCL::OpenCL)
    target_compile_definitions(${X} PRIVATE KERNEL_SOURCE="../../sources/kernels/simple.cl")
    
    if(NVIDIA)
        target_compile_definitions(${X} PRIVATE NVIDIA)
    endif()
endforeach()

# set different types
target_compile_definitions(${SH_TEST_EXEC} PRIVATE TEST_TYPE=short)
target_compile_definitions(${U_TEST_EXEC} PRIVATE TEST_TYPE=unsigned\ int)
target_compile_definitions(${F_TEST_EXEC} PRIVATE TEST_TYPE=float)

# adding tests
add_test(NAME FloatTest COMMAND ${F_TEST_EXEC} ${CMAKE_CURRENT_SOURCE_DIR}/testdata/ftest1.dat)
add_test(NAME UnsignedTest COMMAND ${U_TEST_EXEC} ${CMAKE_CURRENT_SOURCE_DIR}/testdata/utest1.dat)
add_test(NAME ShortTest COMMAND ${SH_TEST_EXEC} ${CMAKE_CURRENT_SOURCE_DIR}/testdata/shtest1.dat)
add_test(NAME FirstIntTest COMMAND ${I_TEST_EXEC} ${CMAKE_CURRENT_SOURCE_DIR}/testdata/test1.dat)
add_test(NAME SecondIntTest COMMAND ${I_TEST_EXEC} ${CMAKE_CURRENT_SOURCE_DIR}/testdata/test2.dat)

SET_TESTS_PROPERTIES(FloatTest UnsignedTest ShortTest FirstIntTest SecondIntTest PROPERTIES LABELS "CORRECT_TESTS")