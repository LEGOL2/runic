import Runic;

class MyApp : public runic::Application {
public:
  void OnInit(runic::Renderer& renderer) override {
    renderer.SetClearColor(0.0f, 0.5f, 0.6f);
  }
  void OnUpdate(runic::Renderer& renderer, float dt) override {}
  void OnRender(runic::Renderer& renderer) override {}
  void OnShutdown(runic::Renderer& renderer) override {}
};

int main() {
  runic::EngineConfig config{
      .window_width = 800,
      .window_height = 600,
      .window_title = "Runic 3D engine",
      .vsync = true,
  };

  runic::Engine engine(config);
  MyApp app;
  engine.Run(app);
}
