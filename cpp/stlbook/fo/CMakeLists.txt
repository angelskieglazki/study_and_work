cmake_minimum_required(VERSION 3.0.0)
project(sequence1 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(sequence1 sequence1.cpp)
add_executable(sequence2 sequence2.cpp)
add_executable(foreach3 foreach3.cpp)
add_executable(bind1 bind1.cpp)
add_executable(compose3 compose3.cpp)
add_executable(meanvalue meanvalue.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
