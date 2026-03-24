module;

#include <glad/glad.h>

export module Runic.Renderer;

namespace runic {
export class Renderer {
public:
  Renderer() { glEnable(GL_DEPTH_TEST); }

  void SetClearColor(float r, float g, float b, float a = 1.0f) {
    glClearColor(r, g, b, a);
  }

  void Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
};
} // namespace runic
