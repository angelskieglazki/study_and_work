cmake_minimum_required(VERSION 3.0.0)
project(sequence1 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(sequence1 sequence1.cpp)
add_executable(sequence2 sequence2.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
