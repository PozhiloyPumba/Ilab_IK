cmake_minimum_required (VERSION 3.0)        							# VERSION

project (PozhiloyPumba_2Q_cache VERSION 2.0)       						# PROJECT TITLE

set (SOURCES main.cpp)                      							# SOURCES *.cpp
set (HEADERS cache.hpp)                     							# HEADERS *.hpp

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")				# flags of compiler

add_executable (2Q_cache ${SOURCES})