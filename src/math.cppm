module;

#include <array>
#include <cmath>

export module Runic.Math;

namespace runic {

export struct Vec3 {
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;

  Vec3 operator+(const Vec3 &o) const noexcept {
    return {x + o.x, y + o.y, z + o.z};
  }
  Vec3 operator-(const Vec3 &o) const noexcept {
    return {x - o.x, y - o.y, z - o.z};
  }
  Vec3 operator*(float s) const noexcept { return {x * s, y * s, z * s}; }

  float Dot(const Vec3 &o) const noexcept {
    return x * o.x + y * o.y + z * o.z;
  }
  Vec3 Cross(const Vec3 &o) const noexcept {
    return {y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x};
  }
  float Length() const noexcept { return std::sqrt(x * x + y * y + z * z); }
  Vec3 Normalize() const noexcept {
    float len = Length();
    if (len < 1e-8f)
      return *this;
    return *this * (1.0f / len);
  }
};

export struct Mat4 {
  // Column-major storage (OpenGL convention)
  // Column c is at data[c*4 .. c*4+3]
  std::array<float, 16> data{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

  Mat4 operator*(const Mat4 &o) const noexcept {
    Mat4 result;
    result.data.fill(0.0f);
    for (int col = 0; col < 4; ++col) {
      for (int row = 0; row < 4; ++row) {
        float sum = 0.0f;
        for (int k = 0; k < 4; ++k) {
          sum += data[k * 4 + row] * o.data[col * 4 + k];
        }
        result.data[col * 4 + row] = sum;
      }
    }
    return result;
  }

  // Build a right-handed view matrix (eye looks at center)
  static Mat4 LookAt(Vec3 eye, Vec3 center, Vec3 up) noexcept {
    Vec3 f = (center - eye).Normalize();
    Vec3 r = f.Cross(up).Normalize();
    Vec3 u = r.Cross(f);
    Mat4 m;
    m.data = {r.x,          u.x,          -f.x,         0.0f,
              r.y,          u.y,          -f.y,         0.0f,
              r.z,          u.z,          -f.z,         0.0f,
              -r.Dot(eye),  -u.Dot(eye),  f.Dot(eye),   1.0f};
    return m;
  }

  // Build a right-handed perspective projection matrix
  static Mat4 Perspective(float fov_radians, float aspect, float near_plane,
                          float far_plane) noexcept {
    float tan_half = std::tan(fov_radians * 0.5f);
    Mat4 m;
    m.data.fill(0.0f);
    m.data[0] = 1.0f / (aspect * tan_half);
    m.data[5] = 1.0f / tan_half;
    m.data[10] = -(far_plane + near_plane) / (far_plane - near_plane);
    m.data[11] = -1.0f;
    m.data[14] = -(2.0f * far_plane * near_plane) / (far_plane - near_plane);
    return m;
  }
};

} // namespace runic
