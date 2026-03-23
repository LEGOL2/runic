export module Runic.Application;

import Runic.Input;
import Runic.Renderer;

namespace runic {
export class AppContext {
public:
  void RequestClose() { should_close_ = true; }
  bool ShouldClose() const { return should_close_; }

private:
  bool should_close_{false};
};

export class Application {
public:
  virtual ~Application() = default;

  virtual void OnInit(AppContext &ctx, Renderer &renderer) {}
  virtual void OnUpdate(AppContext &ctx, Input &input, float dt) {}
  virtual void OnRender(AppContext &ctx, Renderer &renderer) {}
  virtual void OnShutdown(AppContext &ctx) {}
};

} // namespace runic
