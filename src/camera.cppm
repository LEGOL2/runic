export module Runic.Camera;

import Runic.Math;

namespace runic {

export class Camera {
public:
  Camera() = default;

  Camera(Vec3 position, Vec3 target, Vec3 up, float fov_degrees,
         float near_plane, float far_plane)
      : position_(position), target_(target), up_(up),
        fov_degrees_(fov_degrees), near_plane_(near_plane),
        far_plane_(far_plane) {}

  void SetPosition(Vec3 position) noexcept { position_ = position; }
  void SetTarget(Vec3 target) noexcept { target_ = target; }
  void SetUp(Vec3 up) noexcept { up_ = up; }
  void SetFov(float fov_degrees) noexcept { fov_degrees_ = fov_degrees; }
  void SetClipPlanes(float near_plane, float far_plane) noexcept {
    near_plane_ = near_plane;
    far_plane_ = far_plane;
  }

  Vec3 GetPosition() const noexcept { return position_; }

  Mat4 GetViewMatrix() const noexcept {
    return Mat4::LookAt(position_, target_, up_);
  }

  Mat4 GetProjectionMatrix(float aspect_ratio) const noexcept {
    constexpr float kPi = 3.14159265358979323846f;
    float fov_radians = fov_degrees_ * (kPi / 180.0f);
    return Mat4::Perspective(fov_radians, aspect_ratio, near_plane_,
                             far_plane_);
  }

private:
  Vec3 position_ = {0.0f, 0.0f, 3.0f};
  Vec3 target_ = {0.0f, 0.0f, 0.0f};
  Vec3 up_ = {0.0f, 1.0f, 0.0f};
  float fov_degrees_ = 45.0f;
  float near_plane_ = 0.1f;
  float far_plane_ = 100.0f;
};

} // namespace runic
