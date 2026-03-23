module;

#include <GLFW/glfw3.h>
#include <chrono>

export module Runic.Engine;

import Runic.Application;
import Runic.GlfwContext;
import Runic.Input;
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
                                 config.window_title, config.vsync) {
    RegisterCallbacks();
  }

  void Run(Application &app) {
    app.OnInit(app_context_, renderer_);

    using Clock = std::chrono::steady_clock;
    auto last_time = Clock::now();

    while (!window_.ShouldClose() && !app_context_.ShouldClose()) {
      auto now = Clock::now();
      float dt = std::chrono::duration<float>(now - last_time).count();
      last_time = now;

      input_.FlushFrameState();
      window_.PollEvents();

      app.OnUpdate(app_context_, input_, dt);

      renderer_.Clear();
      app.OnRender(app_context_, renderer_);

      window_.SwapBuffers();
    }

    app.OnShutdown(app_context_);
  }

  const Renderer &GetRenderer() const { return renderer_; }
  bool ShouldClose() const { return window_.ShouldClose(); }

private:
  void RegisterCallbacks() {
    glfwSetWindowUserPointer(window_.GetWindow(), &input_);

    glfwSetKeyCallback(
        window_.GetWindow(),
        [](GLFWwindow *w, int key, int /*scancode*/, int action, int /*mods*/) {
          static_cast<Input *>(glfwGetWindowUserPointer(w))
              ->UpdateKeyState(key, action);
        });

    glfwSetMouseButtonCallback(
        window_.GetWindow(),
        [](GLFWwindow *w, int button, int action, int /*mods*/) {
          static_cast<Input *>(glfwGetWindowUserPointer(w))
              ->UpdateMouseButtonState(button, action);
        });

    glfwSetCursorPosCallback(window_.GetWindow(),
                             [](GLFWwindow *w, double x, double y) {
                               static_cast<Input *>(glfwGetWindowUserPointer(w))
                                   ->UpdateMousePosition(x, y);
                             });

    double mx, my;
    glfwGetCursorPos(window_.GetWindow(), &mx, &my);
    input_.InitMousePosition(mx, my);
  }

  runic::AppContext app_context_;
  runic::GlfwContext glfw_context_;
  runic::Window window_;
  runic::Input input_;
  runic::Renderer renderer_;
};
} // namespace runic
