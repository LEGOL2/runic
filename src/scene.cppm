module;

#include <vector>

export module Runic.Scene;

export import Runic.Camera;
export import Runic.Light;
export import Runic.Math;
export import Runic.Mesh;

namespace runic {

export class Scene {
public:
  void SetCamera(Camera camera) { camera_ = std::move(camera); }
  void AddLight(Light light) { lights_.push_back(std::move(light)); }
  void AddMesh(Mesh mesh) { meshes_.push_back(std::move(mesh)); }

  const Camera &GetCamera() const noexcept { return camera_; }
  const std::vector<Light> &GetLights() const noexcept { return lights_; }
  const std::vector<Mesh> &GetMeshes() const noexcept { return meshes_; }

private:
  Camera camera_;
  std::vector<Light> lights_;
  std::vector<Mesh> meshes_;
};

} // namespace runic
