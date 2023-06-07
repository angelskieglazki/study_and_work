cmake_minimum_required(VERSION 3.0.0)
project(observer VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(observer observer.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set_target_properties(observer
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}  -Wall  -O0 -g" # -Wextra -Wpedantic -Werror
)