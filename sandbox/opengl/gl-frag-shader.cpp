#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr int size_x = 800;
constexpr int size_y = 800;

#define SINCOLOR
constexpr const char* verte_name = "shaders/simplest.vert";
#ifdef SINCOLOR
constexpr const char* frag_name = "shaders/sincolor.frag";
#else
constexpr const char* frag_name = "shaders/simplest.frag";
#endif

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

void error_callback(int, const char *err_str) { throw glfw_error(err_str); }

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

GLFWwindow* initialize_window() {
  glfwSetErrorCallback(error_callback);
  glfwInit();
  GLFWwindow* window =
      glfwCreateWindow(size_x, size_y, "OpenGL", nullptr, nullptr);
  assert(window);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw glfw_error("Failed to initialize GLAD");
  }
  return window;
}

GLfloat vertices[] = {
    // positions        // colors
    0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, // top right
    -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 0.0f, // top left
    0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom left
};


std::string read_file(const char* path) {
  std::string code;
  std::ifstream shader_file;
  shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  shader_file.open(path);
  std::stringstream shader_stream;
  shader_stream << shader_file.rdbuf();
  code = shader_stream.str();
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

void do_render(GLuint vao, GLuint program_id) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(program_id);
  set_float(program_id, "time", glfwGetTime());
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

int main() try {
  auto cleanup = [](GLFWwindow*) { glfwTerminate(); };
  using Window_uptr = std::unique_ptr<GLFWwindow, decltype(cleanup)>;
  auto window = Window_uptr(initialize_window(), cleanup);

  GLuint vao;
  GLuint vbo;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        reinterpret_cast<void *>(0 * sizeof(GLfloat)));
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        reinterpret_cast<void *>(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  GLuint program_id = link_shaders();
  while (!glfwWindowShouldClose(window.get())) {
    do_render(vao, program_id);
    glfwSwapBuffers(window.get());
    glfwPollEvents();
  }

  // NOTE: non-exception safe here. Will be fixed in following example
  //       how will you fix it, BTW?
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);

  return 0;
} catch (glsl_error& e) {
  std::cerr << "GLSL error: " << e.what() << std::endl;
  std::cerr << e.shader_log << std::endl;
} catch (glfw_error& e) {
  std::cerr << "GLFW error: " << e.what() << std::endl;
} catch (std::exception& e) {
  std::cerr << "Exception: " << e.what() << std::endl;
} catch (...) {
  std::cerr << "Unknown error" << std::endl;
}