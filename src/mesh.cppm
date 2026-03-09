module;

#include <cstdint>
#include <vector>

export module Runic.Mesh;

import Runic.Math;

namespace runic {

export struct Vertex {
  Vec3 position;
  Vec3 normal;
};

export class Mesh {
public:
  void AddVertex(Vertex vertex) { vertices_.push_back(vertex); }
  void AddIndex(uint32_t index) { indices_.push_back(index); }

  const std::vector<Vertex> &GetVertices() const noexcept { return vertices_; }
  const std::vector<uint32_t> &GetIndices() const noexcept { return indices_; }

private:
  std::vector<Vertex> vertices_;
  std::vector<uint32_t> indices_;
};

} // namespace runic
