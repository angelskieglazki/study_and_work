#include "utils.h"
#include <fstream>
#include <sstream>
#include <iostream>

constexpr const char* verte_name = "./shaders/hello-triangle-transform.vert";
constexpr const char* frag_name = "./shaders/hello-triangle-transform.frag";

std::string read_file(const char* path) {
  std::string code;

  std::ifstream shader_file;

  try {
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    shader_file.open(path);

    if (!shader_file.is_open()) {
      throw std::ios_base::failure("Error: unable to open file");
    }

    std::stringstream shader_stream;
    shader_stream << shader_file.rdbuf();
    code = shader_stream.str();
  } catch (const std::ifstream::failure& e) {
    std::cerr << "Exception opening/reading file: " << e.what() << std::endl;
    throw;
  }
  return code;
}

GLuint install_shader(const std::string& code, GLenum shader_type) {
  GLuint shader_id = glCreateShader(shader_type);
  const char* shader_code = code.c_str();
  glShaderSource(shader_id, 1, &shader_code, nullptr);
  glCompileShader(shader_id);

  GLint success;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    throw glsl_compile_error("Failed to compile shader", shader_id);
  }
  return shader_id;
}

GLuint link_shaders() {
  GLuint program_id = glCreateProgram();
  GLuint vert_id = install_shader(read_file(verte_name), GL_VERTEX_SHADER);
  GLuint frag_id = install_shader(read_file(frag_name), GL_FRAGMENT_SHADER);
  glAttachShader(program_id, vert_id);
  glAttachShader(program_id, frag_id);
  glLinkProgram(program_id);
  GLint success;
  glGetShaderiv(program_id, GL_LINK_STATUS, &success);
  if (!success) {
    throw glsl_link_error("Failed to link shader", program_id);
  }

  glDeleteShader(vert_id);
  glDeleteShader(frag_id);
  return program_id;
}

void set_float(GLuint program_id, const char* name, GLfloat value) {
  GLint location = glGetUniformLocation(program_id, name);
  glUniform1f(location, value);
}

void set_float_matrix4(GLuint program_id, const char* name, GLfloat* values) {
  GLint location = glGetUniformLocation(program_id, name);
  glUniformMatrix4fv(location, 1, GL_FALSE, values);
}