export module Runic.Light;

import Runic.Math;

namespace runic {

export enum class LightType {
  Directional,
  Point,
};

export struct Light {
  LightType type = LightType::Point;
  Vec3 position = {0.0f, 10.0f, 0.0f};
  Vec3 color = {1.0f, 1.0f, 1.0f};
  float intensity = 1.0f;
};

} // namespace runic
