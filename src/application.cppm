export module Runic.Application;

import Runic.Renderer;

namespace runic {
export class Application {
public:
  virtual ~Application() = default;

  virtual void OnInit(Renderer &renderer) {}
  virtual void OnUpdate(float dt) {}
  virtual void OnRender(Renderer &renderer) {}
  virtual void OnShutdown() {}
};
} // namespace runic
