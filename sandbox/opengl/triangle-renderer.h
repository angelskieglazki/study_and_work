#pragma once
#include "shared/utils.h"
#include <glm/fwd.hpp>


#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr int size_x = 800;
constexpr int size_y = 800;



class Renderer {
  GLFWwindow* window_;
  GLuint program_id;
  GLuint vao;
  GLuint vbo;
  GLuint ibo;

  float horizontal_angle = 0.0f;
  float vertical_angle = 0.0f;
  float radius = 3.0f;
  glm::vec3 last_position;

  float fov = 45.0f;
  float aspect = (float)size_x / size_y;
  float near = 0.1f;
  float far = 100.0f;

  bool left_press = false;
  double old_x;
  double old_y;

  bool line_mode = false;

  enum ERotate { kNone, kXRotate, kYRotate, kZRotate };
  ERotate rotate_mode = kNone;
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
