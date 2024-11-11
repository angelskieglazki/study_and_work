#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::string read_file(const char* path);
GLuint install_shader(const std::string& code, GLenum shader_type);
GLuint link_shaders();
void set_float(GLuint program_id, const char* name, GLfloat value);
void set_float_matrix4(GLuint program_id, const char* name, GLfloat* values);

struct glfw_error : public std::runtime_error {
  using runtime_error::runtime_error;
};

struct glsl_error : public std::runtime_error {
  std::string shader_log;
  using runtime_error::runtime_error;
};

struct glsl_compile_error : public glsl_error {
  glsl_compile_error(const char* s, GLuint shader_id) : glsl_error(s) {
    GLint length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
    std::vector<char> shader_logv(length);
    glGetShaderInfoLog(shader_id, length, nullptr, shader_logv.data());
    shader_log.assign(shader_logv.begin(), shader_logv.end());
  }
};

struct glsl_link_error : public glsl_error {
  glsl_link_error(const char* s, GLuint program_id) : glsl_error(s) {
    GLint length;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &length);
    std::vector<char> shader_logv(length);
    glGetProgramInfoLog(program_id, length, nullptr, shader_logv.data());
    shader_log.assign(shader_logv.begin(), shader_logv.end());
  }
};