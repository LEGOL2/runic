module;

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <stdexcept>

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
      }
      window_ = other.window_;
      window_width_ = other.window_width_;
      window_height_ = other.window_height_;
      other.window_ = nullptr;
    }
    return *this;
  }

  Window(int window_width, int window_height, const char *window_title,
         bool vsync)
      : window_width_(window_width), window_height_(window_height) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
#endif
    window_ = glfwCreateWindow(window_width_, window_height_, window_title,
                               nullptr, nullptr);
    if (!window_) {
      throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window_);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
      glfwDestroyWindow(window_);
      throw std::runtime_error("Failed to initialize GLAD");
    }
    if (vsync) {
      glfwSwapInterval(1);
    }
    glViewport(0, 0, window_width_, window_height_);
  }

  ~Window() {
    if (window_) {
      glfwDestroyWindow(window_);
    }
  }

  GLFWwindow *GetWindow() const { return window_; }
  bool ShouldClose() const { return glfwWindowShouldClose(window_); }
  void PollEvents() { glfwPollEvents(); }
  void SwapBuffers() { glfwSwapBuffers(window_); }

private:
  GLFWwindow *window_;
  int window_width_;
  int window_height_;
};
} // namespace runic
