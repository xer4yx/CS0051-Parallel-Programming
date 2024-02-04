cmake_minimum_required(VERSION 3.27)
project(ParallelProg)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab1 nothreads.cpp)
add_executable(lab2 withthreads.cpp)
add_executable(lab3 sequential.cpp)
add_executable(lab4 sequential_withThreads.cpp)
