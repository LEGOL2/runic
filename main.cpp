import Runic;

int main() {
  runic::EngineConfig config{
      .window_width = 800,
      .window_height = 600,
      .window_title = "Runic 3D engine",
      .vsync = true,
  };

  // Build a simple triangle mesh
  runic::Mesh triangle;
  triangle.AddVertex({{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}});
  triangle.AddVertex({{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}});
  triangle.AddVertex({{0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}});
  triangle.AddIndex(0);
  triangle.AddIndex(1);
  triangle.AddIndex(2);

  // Position the camera 3 units back, looking at the origin
  runic::Camera camera{{0.0f, 0.0f, 3.0f},
                       {0.0f, 0.0f, 0.0f},
                       {0.0f, 1.0f, 0.0f},
                       45.0f,
                       0.1f,
                       100.0f};

  // Add a white point light above and to the side of the scene
  runic::Light light;
  light.position = {2.0f, 2.0f, 2.0f};
  light.color = {1.0f, 1.0f, 1.0f};
  light.intensity = 1.0f;

  // Assemble the scene
  runic::Scene scene;
  scene.SetCamera(camera);
  scene.AddLight(light);
  scene.AddMesh(triangle);

  runic::Engine engine(config);
  while (!engine.ShouldClose()) {
    engine.Render(scene);
  }
}
