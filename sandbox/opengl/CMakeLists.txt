cmake_minimum_required(VERSION 3.0.0)
project(test VERSION 0.1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp glad.c)
target_link_libraries(main -lglfw -lGL -ldl)

add_executable(hello-window hello-window.cpp glad.c)
target_link_libraries(hello-window -lglfw -lGL -ldl)

add_executable(hello-triangle hello-triangle.cpp glad.c)
target_link_libraries(hello-triangle -O0 -ggdb -lglfw -lGL -ldl)