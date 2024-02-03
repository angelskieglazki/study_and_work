cmake_minimum_required(VERSION 3.0.0)
project(scope_guard VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(scope_guard_old main-oldstyle.cpp)
add_executable(scope_guard_new main-newstyle.cpp)
add_executable(scope_guard_new_1 main-newstyle-1.cpp)
add_executable(scope_guard_new_2 main-newstyle-2.cpp)
