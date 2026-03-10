module;

// clang-format off
#include <GLFW/glfw3.h>
// clang-format on

#include <cstdlib>
#include <stdio.h>

export module Runic.GlfwContext;

namespace runic {
export class GlfwContext {
public:
  GlfwContext(const GlfwContext &) = delete;
  GlfwContext &operator=(const GlfwContext &) = delete;
  GlfwContext(GlfwContext &&) = delete;
  GlfwContext &operator=(GlfwContext &&) = delete;

  GlfwContext() {
    if (!glfwInit()) {
      std::exit(1);
    }
#ifndef NDEBUG
    glfwSetErrorCallback(ErrorCallback);
#endif
  }

  ~GlfwContext() { glfwTerminate(); }

private:
  static void ErrorCallback(int error, const char *description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
  }
};
} // namespace runic
