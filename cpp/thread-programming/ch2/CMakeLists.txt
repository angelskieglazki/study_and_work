cmake_minimum_required(VERSION 3.0.0)
project(ch2 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(listing2.2 l2.2.cpp)
add_executable(listing2.3 l2.2.cpp)
add_executable(listing2.6 l2.6.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
