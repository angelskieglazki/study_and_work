cmake_minimum_required(VERSION 3.0.0)
project(test VERSION 0.1.0)

add_executable(test main.cpp)
add_executable(test1 nmain.cpp)

set_target_properties(test
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}  -Wall  -O0 -g" # -Wextra -Wpedantic -Werror
)
set_target_properties(test1
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}  -Wall  -O0 -g" # -Wextra -Wpedantic -Werror
)