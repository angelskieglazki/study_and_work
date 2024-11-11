#include <algorithm>
#include <cassert>
#include <fstream>
#include <glm/fwd.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

constexpr int size_x = 800;
constexpr int size_y = 800;

constexpr const char* verte_name = "shaders/models.vert";
constexpr const char* frag_name = "shaders/sincolor.frag";

constexpr glm::vec3 start_pos = {0.0f, 0.0f, 3.0f};
constexpr glm::vec3 start_front = {0.0f, 0.0f, 0.0f};
constexpr glm::vec3 start_up = {0.0f, 1.0f, 0.0f};

constexpr float sensitivity = 0.2f;
constexpr float radius_delta = 0.02f;

GLfloat Vertices[] = {
    // positions        // colors
    // for +Z (cube front)
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,   // top left (-X, +Y)
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,    // top right (+X, +Y)
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,   // bottom right (+X, -Y)
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f,  // bottom left (-X, -Y)

    // for -Z (cube back)
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,   // top left (-X, +Y)
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,    // top right (+X, +Y)
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right (+X, -Y)
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left (-X, -Y)
};

// cube sides to render
GLubyte Indices[] = {
    // those indices better to be in cube order
    // quads
    0, 3, 2, 1, 4, 7, 3, 0, 1, 2, 6, 5, 4, 0, 1, 5, 3, 7, 6, 2, 5, 6, 7, 4,
};

class Renderer {
  GLFWwindow* window_;
  GLuint program_id;
  GLuint vao;
  GLuint vbo;
  GLuint ibo;

  float horizontal_angle = 0.0f;
  float vertical_angle = 0.0f;
  float radius = 3.0f;

  float fov = 45.0f;
  float aspect = (float)size_x / size_y;
  float near = 0.1f;
  float far = 100.0f;

  bool left_press = false;
  double old_x;
  double old_y;

  bool line_mode = false;

 public:
  Renderer(GLFWwindow* wnd);
  Renderer(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer& operator=(Renderer&&) = delete;
  ~Renderer() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
  }
  void display() const;
  void notifyKey(int key, int scancode, int action, int mods);
  void notifyMouseMove(double xpos, double ypos);
  void notifyMouseButton(int button, int action, int mods);
  void notifyaspect(float w, float h) { aspect = w / h; }
};

std::unique_ptr<Renderer> renderer;

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

void error_cb(int, const char* err_str) { throw glfw_error(err_str); }

static void key_cb(GLFWwindow* window, int key, int scancode, int action,
                   int mods) {
  renderer->notifyKey(key, scancode, action, mods);
}

static void cursor_position_cb(GLFWwindow* window, double xpos, double ypos) {
  renderer->notifyMouseMove(xpos, ypos);
}

static void mouse_button_cb(GLFWwindow* window, int button, int action,
                            int mods) {
  renderer->notifyMouseButton(button, action, mods);
}

static void framebuffer_size_cb(GLFWwindow* window, int width, int height) {
  renderer->notifyaspect((float)width, (float)height);
  glViewport(0, 0, width, height);
}

GLFWwindow* initialize_window() {
  glfwSetErrorCallback(error_cb);
  glfwInit();
  GLFWwindow* window =
      glfwCreateWindow(size_x, size_y, "OpenGL", nullptr, nullptr);
  assert(window);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw glfw_error("Failed to initialize GLAD");
  }
  return window;
}

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

void set_float_matrix4(GLuint program_id, const char* name, GLfloat* values) {
  GLint location = glGetUniformLocation(program_id, name);
  glUniformMatrix4fv(location, 1, GL_FALSE, values);
}

Renderer::Renderer(GLFWwindow* wnd)
    : window_(wnd), program_id(link_shaders()) {
  glViewport(0, 0, size_x, size_y);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_CLAMP);
  glEnable(GL_CULL_FACE);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
}

void Renderer::display() const {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(program_id);
  float cur_time = glfwGetTime();
  set_float(program_id, "time", cur_time);

  auto model = glm::mat4(1.0f);
  auto view = glm::mat4(1.0f);
  auto projection = glm::mat4(1.0f);

  float phi = glm::radians(90.0f - vertical_angle);
  float theta = glm::radians(horizontal_angle + 180.0f);

  glm::vec3 Position(-sin(phi) * sin(theta) * radius, cos(phi) * radius,
                     sin(phi) * cos(theta) * radius);

  glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

  view = glm::lookAt(Position, glm::vec3(0.0f, 0.0f, 0.0f), Up);
  projection = glm::perspective(glm::radians(fov), aspect, near, far);

  set_float_matrix4(program_id, "model", glm::value_ptr(model));
  set_float_matrix4(program_id, "view", glm::value_ptr(view));
  set_float_matrix4(program_id, "projection", glm::value_ptr(projection));
  glDrawElements(GL_QUADS, 4 * 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
}

void Renderer::notifyKey(int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    if (key == GLFW_KEY_UP || key == GLFW_KEY_W) {
      radius -= radius_delta;
    }
    if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S) {
      radius += radius_delta;
    }
  }

  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_L) {
      line_mode = !line_mode;
      glad_glPolygonMode(GL_FRONT_AND_BACK, line_mode ? GL_LINE : GL_FILL);
      if (line_mode) {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
      } else {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
      }
    }
  }
}

template <typename T>
T myclamp(T x, T low, T hi) {
  if (x > hi) return hi;
  if (x < low) return low;
  return x;
}

void Renderer::notifyMouseMove(double xpos, double ypos) {
  if (left_press) {
    float xoffset = xpos - old_x;
    float yoffset = ypos - old_y;
    // std::cout << "old_x:" << old_x << ", old_y:" << old_y << std::endl;
    // std::cout << "xpos:" << xpos << ", ypos:" << ypos << std::endl;
    // std::cout << "xoffset:" << xoffset << ", yoffset:" << yoffset << std::endl;
    old_x = xpos;
    old_y = ypos;
    horizontal_angle += xoffset * sensitivity;
    vertical_angle += yoffset * sensitivity;
    std::cout << "horizontal_angle:" << horizontal_angle
              << ", vertical_angle:" << vertical_angle << std::endl;
    vertical_angle = myclamp(vertical_angle, -89.0f, 89.0f);
    horizontal_angle = myclamp(horizontal_angle, -89.0f, 89.0f);
  }
}

void Renderer::notifyMouseButton(int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    left_press = true;
    glfwGetCursorPos(window_, &old_x, &old_y);
  }

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    left_press = false;
  }
}

int main() try {
  auto Cleanup = [](GLFWwindow*) { glfwTerminate(); };
  using Window_uptr = std::unique_ptr<GLFWwindow, decltype(Cleanup)>;
  Window_uptr window(initialize_window(), Cleanup);
  renderer = std::make_unique<Renderer>(window.get());

  glfwSetKeyCallback(window.get(), key_cb);
  glfwSetMouseButtonCallback(window.get(), mouse_button_cb);
  glfwSetCursorPosCallback(window.get(), cursor_position_cb);

  while (!glfwWindowShouldClose(window.get())) {
    renderer->display();
    glfwSwapBuffers(window.get());
    glfwPollEvents();
  }

  renderer.reset();

} catch (glsl_error& E) {
  std::cout << "GLSL error: " << E.what() << std::endl;
  std::cout << E.shader_log << std::endl;
} catch (glfw_error& E) {
  std::cout << "GLFW error: " << E.what() << std::endl;
} catch (std::exception& E) {
  std::cout << "Standard error: " << E.what() << std::endl;
} catch (...) {
  std::cout << "Unknown error\n";
}