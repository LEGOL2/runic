import Runic;

#include <optional>
#include <span>

class MyApp : public runic::Application {
public:
  void OnInit(runic::AppContext &ctx, runic::Renderer &renderer) override {
    renderer.SetClearColor(0.0f, 0.5f, 0.6f);

    runic::Shader vert, frag;
    vert.Compile("assets/triangle.vert",
                 runic::Shader::ShaderType::VERTEX_SHADER);
    frag.Compile("assets/triangle.frag",
                 runic::Shader::ShaderType::FRAGMENT_SHADER);
    program_.Build(vert, frag);
    constexpr float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
    };
    mesh_.emplace(std::span{vertices});
  }

  void OnUpdate(runic::AppContext &ctx, runic::Input &input,
                float dt) override {
    if (input.IsKeyPressed(runic::Key::ESCAPE)) {
      ctx.RequestClose();
    }
  }

  void OnRender(runic::AppContext &ctx, runic::Renderer &renderer) override {
    program_.Bind();
    mesh_->Draw();
  }
  void OnShutdown(runic::AppContext &ctx) override {}

private:
  runic::Program program_;
  std::optional<runic::Mesh> mesh_;
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
