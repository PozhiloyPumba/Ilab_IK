set (TEST_SRC 
    testing.cpp
)

# for template tests I need different binary files
set (F_TEST_EXEC "correct_float_test")
set (I_TEST_EXEC "correct_int_test")
set (U_TEST_EXEC "correct_unsigned_test")
set (SH_TEST_EXEC "correct_short_test")

# foreach file linking with OpenCL, targeting include directory...
list(APPEND TESTS_EXEC ${F_TEST_EXEC} ${I_TEST_EXEC} ${U_TEST_EXEC} ${SH_TEST_EXEC})

foreach(X IN LISTS TESTS_EXEC)
    add_executable(${X} ${TEST_SRC})
    target_include_directories(${X} PRIVATE ../include)
    target_link_libraries(${X} OpenCL::OpenCL)
    
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
add_test(NAME FirstIntTest COMMAND ${I_TEST_EXEC} ${CMAKE_CURRENT_SOURCE_DIR}/testdata/test3.dat)
add_test(NAME SecondIntTest COMMAND ${I_TEST_EXEC} ${CMAKE_CURRENT_SOURCE_DIR}/testdata/test4.dat)