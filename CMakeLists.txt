cmake_minimum_required(VERSION 3.28)
project(OpenMP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
set(CMAKE_CXX_STANDARD 17)

add_executable(OpenMP main.cpp)
