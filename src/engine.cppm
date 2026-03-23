module;

#include <chrono>

export module Runic.Engine;

import Runic.Application;
import Runic.GlfwContext;
import Runic.Renderer;
import Runic.Window;

namespace runic {
export struct EngineConfig {
  int window_width;
  int window_height;
  const char *window_title;
  bool vsync;
};

export class Engine {
public:
  Engine(EngineConfig config)
      : glfw_context_(), window_(config.window_width, config.window_height,
                                 config.window_title, config.vsync) {}

  void Run(Application &app) {
    app.OnInit(renderer_);

    using Clock = std::chrono::steady_clock;
    auto last_time = Clock::now();

    while (!window_.ShouldClose()) {
      auto now = Clock::now();
      float dt = std::chrono::duration<float>(now - last_time).count();
      last_time = now;

      app.OnUpdate(dt);

      renderer_.Clear();
      app.OnRender(renderer_);

      window_.EndFrame();
    }

    app.OnShutdown();
  }

  const Renderer &GetRenderer() const { return renderer_; }
  bool ShouldClose() const { return window_.ShouldClose(); }

private:
  runic::GlfwContext glfw_context_;
  runic::Window window_;
  runic::Renderer renderer_;
};
} // namespace runic
