module;

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <cstdlib>
#include <stdio.h>

export module Runic.Window;

namespace runic {
export class Window {
public:
  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  Window(Window &&other) noexcept
      : window_(other.window_), window_width_(other.window_width_),
        window_height_(other.window_height_) {
    other.window_ = nullptr;
  }

  Window &operator=(Window &&other) noexcept {
    if (this != &other) {
      if (window_) {
        glfwDestroyWindow(window_);
        glfwTerminate();
      }
      window_ = other.window_;
      window_width_ = other.window_width_;
      window_height_ = other.window_height_;
      other.window_ = nullptr;
    }
    return *this;
  }

  Window(size_t window_width, size_t window_height, const char *window_title,
         bool vsync)
      : window_width_(window_width), window_height_(window_height) {
    if (!glfwInit()) {
      std::exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifndef NDEBUG
    glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
    glfwSetErrorCallback(ErrorCallback);
#endif
    window_ = glfwCreateWindow(window_width_, window_height_, window_title,
                               nullptr, nullptr);
    if (!window_) {
      glfwTerminate();
      std::exit(1);
    }

    glfwMakeContextCurrent(window_);
    gladLoadGL();
    if (vsync) {
      glfwSwapInterval(1);
    }
    glViewport(0, 0, window_width_, window_height_);
  }

  ~Window() {
    if (window_) {
      glfwDestroyWindow(window_);
      glfwTerminate();
    }
  }

  bool ShouldClose() const { return glfwWindowShouldClose(window_); }

  void EndFrame() {
    glfwSwapBuffers(window_);
    glfwPollEvents();
  }

private:
  static void ErrorCallback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
  }

  GLFWwindow *window_;
  size_t window_width_;
  size_t window_height_;
};
} // namespace runic
