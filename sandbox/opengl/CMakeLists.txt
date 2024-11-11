cmake_minimum_required(VERSION 3.0.0)
project(test VERSION 0.1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp glad.c)
target_link_libraries(main -lglfw -lGL -ldl)

add_executable(hello-window hello-window.cpp glad.c)
target_link_libraries(hello-window -lglfw -lGL -ldl)

add_executable(hello-triangle hello-triangle.cpp glad.c)
target_link_libraries(hello-triangle -lglfw -lGL -ldl)

add_executable(hello-triangle-transform hello-triangle-transform.cpp shared/utils.cpp triangle-renderer.cpp glad.c)
target_link_libraries(hello-triangle-transform -lglfw -lGL -ldl)

add_executable(gl-extensions gl-extensions.cpp glad.c)
target_link_libraries(gl-extensions -lglfw -lGL -ldl)

add_executable(gl-frag-shader gl-frag-shader.cpp glad.c)
target_link_libraries(gl-frag-shader -lglfw -lGL -ldl)

add_executable(gl-motion gl-motion.cpp glad.c)
target_link_libraries(gl-motion -O0 -ggdb -lglfw -lGL -ldl)

file(COPY "${CMAKE_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")