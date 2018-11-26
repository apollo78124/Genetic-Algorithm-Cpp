cmake_minimum_required(VERSION 3.12)
project(Assingment2cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
add_executable(Assingment2cpp main.cpp Genetics.cpp Genetics.hpp)