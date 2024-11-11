#include "triangle-renderer.h"
#include <iostream>

#include <array>



constexpr float sensitivity = 0.2f;
constexpr float radius_delta = 0.02f;

std::array<float, 9 * 2> vertices = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // red // bottom left
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // green // bottom right
    0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f,  // blue // top right
};

std::array<uint32_t, 3> indices = {0, 2, 1};  // Исправлено количество индексов


Renderer::Renderer(GLFWwindow* wnd) : window_(wnd), program_id(link_shaders()) {
  glViewport(0, 0, size_x, size_y);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_CLAMP);
  glEnable(GL_CULL_FACE);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
               GL_STATIC_DRAW);

  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
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

  glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

  // if (rotate_mode == kXRotate) {
  //   model = glm::rotate(model, glm::radians(horizontal_angle), glm::vec3(1.0f, 0.0f, 0.0f));
  // } else if (rotate_mode == kYRotate) {
  //   model = glm::rotate(model, glm::radians(vertical_angle), glm::vec3(0.0f, 1.0f, 0.0f));
  // } else if (rotate_mode == kZRotate) {
  //   model = glm::rotate(model, glm::radians(horizontal_angle), glm::vec3(0.0f, 0.0f, 1.0f));
  // }

  view = glm::lookAt(last_position, glm::vec3(0.0f, 0.0f, 0.0f), Up);

  if (rotate_mode == kXRotate) {
    view = glm::rotate(view, glm::radians(horizontal_angle), glm::vec3(1.0f, 0.0f, 0.0f));
  } else if (rotate_mode == kYRotate) {
    view = glm::rotate(view, glm::radians(vertical_angle), glm::vec3(0.0f, 1.0f, 0.0f));
  } else if (rotate_mode == kZRotate) {
    view = glm::rotate(view, glm::radians(horizontal_angle), glm::vec3(0.0f, 0.0f, 1.0f));
  }

  projection = glm::perspective(glm::radians(fov), aspect, near, far);

  set_float_matrix4(program_id, "model", glm::value_ptr(model));
  set_float_matrix4(program_id, "view", glm::value_ptr(view));
  set_float_matrix4(program_id, "projection", glm::value_ptr(projection));
  // glDrawElements(GL_QUADS, 4 * 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
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
    } else if (key == GLFW_KEY_LEFT_CONTROL) {
      rotate_mode = kZRotate;
    } else if (key == GLFW_KEY_LEFT_SHIFT) {
      rotate_mode = kXRotate;
    } else if (key == GLFW_KEY_LEFT_ALT) {
      rotate_mode = kYRotate;
    }
  } else if (action == GLFW_RELEASE) {
    rotate_mode = kNone;
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
    // std::cout << "xoffset:" << xoffset << ", yoffset:" << yoffset <<
    // std::endl;
    old_x = xpos;
    old_y = ypos;
    horizontal_angle += xoffset * sensitivity;
    vertical_angle += yoffset * sensitivity;

    std::cout << "horizontal_angle:" << horizontal_angle
              << ", vertical_angle:" << vertical_angle << std::endl;
    // vertical_angle = myclamp(vertical_angle, -89.0f, 89.0f);
    // horizontal_angle = myclamp(horizontal_angle, -89.0f, 89.0f);
  }
  if (rotate_mode == kNone) {
    float phi = glm::radians(90.0f - vertical_angle);
    float theta = glm::radians(horizontal_angle + 180.0f);

    last_position =
        glm::vec3(-sin(phi) * sin(theta) * radius, cos(phi) * radius,
                  sin(phi) * cos(theta) * radius);
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