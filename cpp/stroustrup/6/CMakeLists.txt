cmake_minimum_required(VERSION 3.0.0)
project(test VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(ex18 ex18.cpp)
set_target_properties(ex18
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}  -Wall  -O0 -g" # -Wextra -Wpedantic -Werror
)

add_executable(ex20 ex20.cpp)
set_target_properties(ex20
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}  -Wall  -O0 -g" # -Wextra -Wpedantic -Werror
)