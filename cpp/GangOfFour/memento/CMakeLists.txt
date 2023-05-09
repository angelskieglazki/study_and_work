cmake_minimum_required(VERSION 3.0.0)
project(memento VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(memento1 memento1.cpp)
add_executable(memento2 memento2.cpp)
add_executable(memento_command memento_command.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
