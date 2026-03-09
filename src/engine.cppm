module;

#include <glad/glad.h>

#include <cstddef>
#include <cstdio>
#include <vector>

export module Runic.Engine;

import Runic.Scene;
import Runic.Window;

namespace runic {

// Internal GPU representation of an uploaded mesh (VAO/VBO/EBO).
// Not exported — users interact with Mesh instead.
struct GpuMesh {
  unsigned int vao = 0;
  unsigned int vbo = 0;
  unsigned int ebo = 0;
  int index_count = 0;

  GpuMesh() = default;
  GpuMesh(const GpuMesh &) = delete;
  GpuMesh &operator=(const GpuMesh &) = delete;

  GpuMesh(GpuMesh &&o) noexcept
      : vao(o.vao), vbo(o.vbo), ebo(o.ebo), index_count(o.index_count) {
    o.vao = 0;
    o.vbo = 0;
    o.ebo = 0;
  }

  GpuMesh &operator=(GpuMesh &&o) noexcept {
    if (this != &o) {
      Destroy();
      vao = o.vao;
      vbo = o.vbo;
      ebo = o.ebo;
      index_count = o.index_count;
      o.vao = 0;
      o.vbo = 0;
      o.ebo = 0;
    }
    return *this;
  }

  ~GpuMesh() { Destroy(); }

  void Destroy() noexcept {
    if (vao) {
      glDeleteVertexArrays(1, &vao);
      vao = 0;
    }
    if (vbo) {
      glDeleteBuffers(1, &vbo);
      vbo = 0;
    }
    if (ebo) {
      glDeleteBuffers(1, &ebo);
      ebo = 0;
    }
  }
};

} // namespace runic

namespace {

const char *kVertexShaderSrc = R"glsl(
#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vFragPos;
out vec3 vNormal;

void main() {
    vFragPos = vec3(uModel * vec4(aPos, 1.0));
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    gl_Position = uProjection * uView * vec4(vFragPos, 1.0);
}
)glsl";

const char *kFragmentShaderSrc = R"glsl(
#version 460 core
in vec3 vFragPos;
in vec3 vNormal;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

#define MAX_LIGHTS 8
uniform Light uLights[MAX_LIGHTS];
uniform int uNumLights;
uniform vec3 uViewPos;

out vec4 FragColor;

void main() {
    vec3 norm = normalize(vNormal);
    vec3 result = vec3(0.15); // ambient term
    for (int i = 0; i < uNumLights && i < MAX_LIGHTS; ++i) {
        vec3 lightDir = normalize(uLights[i].position - vFragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        result += diff * uLights[i].color * uLights[i].intensity;
    }
    FragColor = vec4(clamp(result, 0.0, 1.0), 1.0);
}
)glsl";

unsigned int CompileShader(unsigned int type, const char *src) {
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);
  int success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char log[512];
    glGetShaderInfoLog(shader, 512, nullptr, log);
    fprintf(stderr, "Shader compile error: %s\n", log);
  }
  return shader;
}

unsigned int CreateShaderProgram() {
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, kVertexShaderSrc);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, kFragmentShaderSrc);
  unsigned int program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  int success = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char log[512];
    glGetProgramInfoLog(program, 512, nullptr, log);
    fprintf(stderr, "Shader link error: %s\n", log);
  }
  glDeleteShader(vs);
  glDeleteShader(fs);
  return program;
}

runic::GpuMesh UploadMesh(const runic::Mesh &mesh) {
  runic::GpuMesh gpu;
  glGenVertexArrays(1, &gpu.vao);
  glGenBuffers(1, &gpu.vbo);
  glGenBuffers(1, &gpu.ebo);

  glBindVertexArray(gpu.vao);

  const auto &verts = mesh.GetVertices();
  glBindBuffer(GL_ARRAY_BUFFER, gpu.vbo);
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(verts.size() * sizeof(runic::Vertex)),
               verts.data(), GL_STATIC_DRAW);

  const auto &indices = mesh.GetIndices();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu.ebo);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
      indices.data(), GL_STATIC_DRAW);

  // position attribute: location 0, vec3
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                        static_cast<GLsizei>(sizeof(runic::Vertex)),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  // normal attribute: location 1, vec3 (offset = sizeof Vec3 = 3 floats)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                        static_cast<GLsizei>(sizeof(runic::Vertex)),
                        reinterpret_cast<void *>(sizeof(runic::Vec3)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  gpu.index_count = static_cast<int>(indices.size());
  return gpu;
}

} // anonymous namespace

namespace runic {

export struct EngineConfig {
  size_t window_width;
  size_t window_height;
  const char *window_title;
  bool vsync;
};

export class Engine {
public:
  Engine(const Engine &) = delete;
  Engine &operator=(const Engine &) = delete;
  Engine(Engine &&) = delete;
  Engine &operator=(Engine &&) = delete;

  explicit Engine(EngineConfig config)
      : window_(static_cast<int>(config.window_width),
                static_cast<int>(config.window_height), config.window_title,
                config.vsync),
        shader_program_(CreateShaderProgram()),
        window_width_(static_cast<int>(config.window_width)),
        window_height_(static_cast<int>(config.window_height)) {
    glEnable(GL_DEPTH_TEST);
  }

  ~Engine() {
    if (shader_program_) {
      glDeleteProgram(shader_program_);
      shader_program_ = 0;
    }
  }

  void Render(const Scene &scene) {
    SyncGpuMeshes(scene);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader_program_);

    // Camera uniforms
    const Camera &cam = scene.GetCamera();
    float aspect =
        static_cast<float>(window_width_) / static_cast<float>(window_height_);
    SetUniformMat4("uView", cam.GetViewMatrix());
    SetUniformMat4("uProjection", cam.GetProjectionMatrix(aspect));

    Vec3 cam_pos = cam.GetPosition();
    glUniform3f(glGetUniformLocation(shader_program_, "uViewPos"), cam_pos.x,
                cam_pos.y, cam_pos.z);

    // Light uniforms
    // TODO: cache uniform locations to avoid per-frame string lookups.
    const auto &lights = scene.GetLights();
    int num_lights = static_cast<int>(lights.size());
    if (num_lights > 8)
      num_lights = 8;
    glUniform1i(glGetUniformLocation(shader_program_, "uNumLights"), num_lights);
    for (int i = 0; i < num_lights; ++i) {
      char name[64];
      snprintf(name, sizeof(name), "uLights[%d].position", i);
      glUniform3f(glGetUniformLocation(shader_program_, name),
                  lights[i].position.x, lights[i].position.y,
                  lights[i].position.z);
      snprintf(name, sizeof(name), "uLights[%d].color", i);
      glUniform3f(glGetUniformLocation(shader_program_, name),
                  lights[i].color.x, lights[i].color.y, lights[i].color.z);
      snprintf(name, sizeof(name), "uLights[%d].intensity", i);
      glUniform1f(glGetUniformLocation(shader_program_, name),
                  lights[i].intensity);
    }

    // Identity model matrix (per-mesh transforms can be added later)
    Mat4 model;
    SetUniformMat4("uModel", model);

    // Draw all meshes
    for (const auto &gpu_mesh : gpu_meshes_) {
      if (gpu_mesh.index_count == 0)
        continue;
      glBindVertexArray(gpu_mesh.vao);
      glDrawElements(GL_TRIANGLES, gpu_mesh.index_count, GL_UNSIGNED_INT,
                     nullptr);
    }
    glBindVertexArray(0);

    window_.EndFrame();
  }

  bool ShouldClose() const { return window_.ShouldClose(); }

private:
  // Upload GPU meshes whenever the scene's mesh count changes.
  // TODO: track individual mesh changes to avoid re-uploading unchanged meshes.
  void SyncGpuMeshes(const Scene &scene) {
    const auto &meshes = scene.GetMeshes();
    if (gpu_meshes_.size() != meshes.size()) {
      gpu_meshes_.clear();
      for (const auto &mesh : meshes) {
        gpu_meshes_.push_back(UploadMesh(mesh));
      }
    }
  }

  void SetUniformMat4(const char *name, const Mat4 &mat) {
    int loc = glGetUniformLocation(shader_program_, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, mat.data.data());
  }

  runic::Window window_;
  unsigned int shader_program_ = 0;
  int window_width_ = 0;
  int window_height_ = 0;
  std::vector<GpuMesh> gpu_meshes_;
};

} // namespace runic
