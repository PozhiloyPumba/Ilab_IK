cmake_minimum_required (VERSION 3.0)        							# VERSION

project (PozhiloyPumba_triangles VERSION 2.0)       					# PROJECT TITLE

set (SOURCES main.cpp)                      							# SOURCES *.cpp
set (HEADERS triangles.hpp vector.hpp octotree.hpp)                     # HEADERS *.hpp

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")				# flags of compiler

add_executable (triangles ${SOURCES})