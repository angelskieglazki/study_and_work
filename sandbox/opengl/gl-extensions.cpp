#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr int size_x = 800;
constexpr int size_y = 800;

struct glfw_error : public std::runtime_error {
  using runtime_error::runtime_error;
};

[[noreturn]] void error_callback(int, const char* description) {
  throw glfw_error(description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
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
    // positions        // colors (not used in this example)
    -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 0.0f, // top left
    0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, // top right
    0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom left
};

void do_render(GLuint vao) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindVertexArray(vao);
  glDrawArrays(GL_QUADS, 0, 4);
}

int main() try {
  auto cleanup = [](GLFWwindow*) { glfwTerminate(); };
  using Window_uptr = std::unique_ptr<GLFWwindow, decltype(cleanup)>;
  auto window = Window_uptr(initialize_window(), cleanup);

  GLuint vao;
  GLuint vbo;

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  while (!glfwWindowShouldClose(window.get())) {
    do_render(vao);
    glfwSwapBuffers(window.get());
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);

  return 0;
} catch (const glfw_error& e) {
  std::cerr << "GLFW error: " << e.what() << std::endl;
} catch (const std::exception& e) {
  std::cerr << "Error: " << e.what() << std::endl;
} catch (...) {
  std::cerr << "Unknown error" << std::endl;
}