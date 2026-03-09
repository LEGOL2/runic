module;

#include <glad/glad.h>

export module Runic.Engine;

import Runic.GlfwContext;
import Runic.Window;

namespace runic {
export struct EngineConfig {
  size_t window_width;
  size_t window_height;
  const char *window_title;
  bool vsync;
};

export class Engine {
public:
  Engine(EngineConfig config)
      : glfw_context_(),
        window_(config.window_width, config.window_height, config.window_title,
                config.vsync) {}

  void Render() {
    glClearColor(0.0f, 0.5f, 0.6f, 1.0f);

    // begin frame
    glClear(GL_COLOR_BUFFER_BIT);
    // render

    // end frame
    window_.EndFrame();
  }

  bool ShouldClose() const {
    return window_.ShouldClose();
  }

private:
  runic::GlfwContext glfw_context_;
  runic::Window window_;
};
} // namespace runic
