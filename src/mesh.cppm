module;

#include <glad/glad.h>

#include <span>

export module Runic.Mesh;

namespace runic {
export class Mesh {
public:
  Mesh(const Mesh &) = delete;
  Mesh &operator=(const Mesh &) = delete;

  Mesh(Mesh &&other) noexcept
      : vao_(other.vao_), vbo_(other.vbo_), ebo_(other.ebo_),
        index_count_(other.index_count_), vertex_count_(other.vertex_count_) {
    other.vao_ = other.vbo_ = other.ebo_ = 0;
    other.index_count_ = other.vertex_count_ = 0;
  }

  Mesh &operator=(Mesh &&other) noexcept {
    if (this != &other) {
      Delete();
      vao_ = other.vao_;
      vbo_ = other.vbo_;
      ebo_ = other.ebo_;
      index_count_ = other.index_count_;
      vertex_count_ = other.vertex_count_;
      other.vao_ = other.vbo_ = other.ebo_ = 0;
      other.index_count_ = other.vertex_count_ = 0;
    }
    return *this;
  }

  explicit Mesh(std::span<const float> vertices)
      : vertex_count_(vertices.size() / 3) {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertices.size_bytes()),
                 vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
  }

  Mesh(std::span<const float> vertices, std::span<const unsigned int> indices)
      : index_count_(indices.size()) {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertices.size_bytes()),
                 vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(indices.size_bytes()), indices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
  }

  ~Mesh() { Delete(); }

  void Draw() const {
    if (vao_ == 0)
      return;

    glBindVertexArray(vao_);

    if (ebo_) {
      if (index_count_ == 0)
        return;
      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index_count_),
                     GL_UNSIGNED_INT, nullptr);
    } else {
      if (vertex_count_ == 0)
        return;
      glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertex_count_));
    }

    glBindVertexArray(0);
  }

private:
  void Delete() {
    if (vao_)
      glDeleteVertexArrays(1, &vao_);
    if (vbo_)
      glDeleteBuffers(1, &vbo_);
    if (ebo_)
      glDeleteBuffers(1, &ebo_);
    vao_ = vbo_ = ebo_ = 0;
  }

  GLuint vao_{};
  GLuint vbo_{};
  GLuint ebo_{};
  std::size_t index_count_{};
  std::size_t vertex_count_{};
};
} // namespace runic
