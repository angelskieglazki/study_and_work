cmake_minimum_required(VERSION 3.0.0)
project(test VERSION 0.1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

# add_executable(test main.cpp)
add_executable(affinity_test affinity_test.cpp)