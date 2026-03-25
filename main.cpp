import Runic;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <optional>
#include <span>

class MyApp : public runic::Application {
public:
  void OnInit(runic::AppContext &ctx, runic::Renderer &renderer) override {
    renderer.SetClearColor(0.1f, 0.1f, 0.1f);

    runic::Shader vert, frag;
    vert.Compile("assets/cube.vert", runic::Shader::ShaderType::VERTEX_SHADER);
    frag.Compile("assets/cube.frag",
                 runic::Shader::ShaderType::FRAGMENT_SHADER);
    program_.Build(vert, frag);

    constexpr float vertices[] = {
        -0.5f, -0.5f, -0.5f, // 0 back-bottom-left
        0.5f,  -0.5f, -0.5f, // 1 back-bottom-right
        0.5f,  0.5f,  -0.5f, // 2 back-top-right
        -0.5f, 0.5f,  -0.5f, // 3 back-top-left
        -0.5f, -0.5f, 0.5f,  // 4 front-bottom-left
        0.5f,  -0.5f, 0.5f,  // 5 front-bottom-right
        0.5f,  0.5f,  0.5f,  // 6 front-top-right
        -0.5f, 0.5f,  0.5f,  // 7 front-top-left
    };

    constexpr unsigned int indices[] = {
        0, 1, 2, 2, 3, 0, // back
        4, 5, 6, 6, 7, 4, // front
        0, 4, 7, 7, 3, 0, // left
        1, 5, 6, 6, 2, 1, // right
        3, 2, 6, 6, 7, 3, // top
        0, 1, 5, 5, 4, 0, // bottom
    };

    mesh_.emplace(std::span{vertices}, std::span{indices});

    float aspect = 800.0f / 600.0f;
    projection_ = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
  }

  void OnUpdate(runic::AppContext &ctx, runic::Input &input,
                float dt) override {
    if (input.IsKeyPressed(runic::Key::ESCAPE)) {
      ctx.RequestClose();
    }

    elapsed_ += dt;
  }

  void OnRender(runic::AppContext &ctx, runic::Renderer &renderer) override {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
    model = glm::rotate(model, elapsed_ * glm::radians(50.0f),
                        glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, elapsed_ * glm::radians(30.0f),
                        glm::vec3(0.0f, 1.0f, 0.0f));

    program_.Bind();
    program_.SetUniform("uModel", model);
    program_.SetUniform("uProjection", projection_);
    mesh_->Draw();
  }

  void OnShutdown(runic::AppContext &ctx) override {}

private:
  runic::Program program_;
  std::optional<runic::Mesh> mesh_;
  glm::mat4 projection_{};
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
