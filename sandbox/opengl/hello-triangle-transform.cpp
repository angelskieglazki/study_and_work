#include <sys/types.h>
#include <cassert>
#include <iostream>
#include <memory>
#include "shared/utils.h"
#include "triangle-renderer.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);



std::unique_ptr<Renderer> renderer;

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
