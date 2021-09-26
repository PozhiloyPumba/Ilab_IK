cmake_minimum_required (VERSION 3.0)        							# VERSION

project (PozhiloyPumba_test VERSION 2.0)       						# PROJECT TITLE

set (SOURCES unit_tests.cpp)                      							# SOURCES *.cpp

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")				# flags of compiler

add_executable (a.out ${SOURCES})