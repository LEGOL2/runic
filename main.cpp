import Runic;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

    // Translation
    if (input.IsKeyDown(runic::Key::W))
      position_.y += kMoveSpeed * dt;
    if (input.IsKeyDown(runic::Key::S))
      position_.y -= kMoveSpeed * dt;
    if (input.IsKeyDown(runic::Key::A))
      position_.x -= kMoveSpeed * dt;
    if (input.IsKeyDown(runic::Key::D))
      position_.x += kMoveSpeed * dt;

    // Rotation
    if (input.IsKeyDown(runic::Key::Q))
      rotation_ += kRotateSpeed * dt;
    if (input.IsKeyDown(runic::Key::E))
      rotation_ -= kRotateSpeed * dt;

    // Scale
    if (input.IsKeyDown(runic::Key::Z))
      scale_ += kScaleSpeed * dt;
    if (input.IsKeyDown(runic::Key::X))
      scale_ -= kScaleSpeed * dt;
    scale_ = glm::max(scale_, 0.1f);

    elapsed_ += dt;
  }

  void OnRender(runic::AppContext &ctx, runic::Renderer &renderer) override {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position_, 0.0f));
    model = glm::rotate(model, rotation_, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scale_, scale_, 1.0f));

    program_.Bind();
    program_.SetUniform("uTime", elapsed_);
    program_.SetUniform("uModel", model);
    mesh_->Draw();
  }

  void OnShutdown(runic::AppContext &ctx) override {}

private:
  static constexpr float kMoveSpeed = 1.0f;
  static constexpr float kRotateSpeed = 2.0f;
  static constexpr float kScaleSpeed = 0.5f;

  runic::Program program_;
  std::optional<runic::Mesh> mesh_;

  glm::vec2 position_ = {0.0f, 0.0f};
  float rotation_ = 0.0f;
  float scale_ = 1.0f;
  float elapsed_ = 0.0f;
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
