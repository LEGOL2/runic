import Runic;

class MyApp : public runic::Application {
public:
  void OnInit(runic::AppContext &ctx, runic::Renderer &renderer) override {
    renderer.SetClearColor(0.0f, 0.5f, 0.6f);
  }

  void OnUpdate(runic::AppContext &ctx, runic::Input &input, float dt) override {
    if (input.IsKeyPressed(runic::Key::ESCAPE)) {
      ctx.RequestClose();
    }
  }

  void OnRender(runic::AppContext &ctx, runic::Renderer &renderer) override {}
  void OnShutdown(runic::AppContext &ctx) override {}
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
