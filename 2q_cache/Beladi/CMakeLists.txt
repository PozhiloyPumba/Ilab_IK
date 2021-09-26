cmake_minimum_required (VERSION 3.0)        							# VERSION

project (PozhiloyPumba_Beladi VERSION 2.0)       						# PROJECT TITLE

set (SOURCES beladi.cpp)                      							# SOURCES *.cpp

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")				# flags of compiler

add_executable (a.out ${SOURCES})