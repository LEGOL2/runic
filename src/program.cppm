module;

#include <glad/glad.h>
#include <stdexcept>
#include <string>

export module Runic.Program;

import Runic.Utils;

namespace runic {
export class Shader final {
public:
  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;

  Shader(Shader &&other) noexcept : shader_id_(other.shader_id_) {
    other.shader_id_ = INVALID_SHADER_ID;
  }

  Shader &operator=(Shader &&other) noexcept {
    if (this != &other) {
      Delete();
      shader_id_ = other.shader_id_;
      other.shader_id_ = INVALID_SHADER_ID;
    }

    return *this;
  }

  Shader() : shader_id_(INVALID_SHADER_ID) {}

  ~Shader() { Delete(); }

  void Delete() {
    if (shader_id_ != INVALID_SHADER_ID) {
      glDeleteShader(shader_id_);
      shader_id_ = INVALID_SHADER_ID;
    }
  }

  GLuint GetID() const { return shader_id_; }
  bool IsNull() const { return shader_id_ == INVALID_SHADER_ID; }

  bool Compile(const std::string &shader_file, GLenum shader_type) {
    Delete();
    shader_id_ = glCreateShader(shader_type);
    if (shader_id_ == INVALID_SHADER_ID) {
      throw std::runtime_error("Failed to create shader");
    }
    auto source_code = runic::utils::LoadSource(shader_file);
    auto *src = source_code.c_str();
    glShaderSource(shader_id_, 1, &src, nullptr);
    glCompileShader(shader_id_);

    GLint result{};
    glGetShaderiv(shader_id_, GL_COMPILE_STATUS, &result);

    GLint info_log_length{};
    glGetShaderiv(shader_id_, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 1) {
      std::string compiler_message(static_cast<std::size_t>(info_log_length),
                                   0);
      GLint log_length{};
      glGetShaderInfoLog(shader_id_,
                         static_cast<GLsizei>(compiler_message.length()),
                         &log_length, compiler_message.data());
      compiler_message.resize(static_cast<std::size_t>(log_length));

      if (result != GL_TRUE) {
        throw std::runtime_error("Cannot compile shader: " + compiler_message);
      }
    }

    if (result) {
      GLint stype;
      glGetShaderiv(shader_id_, GL_SHADER_TYPE, &stype);
      if (stype != static_cast<GLint>(shader_type)) {
        throw std::runtime_error("Incorrect shader type");
      }
    }

    return result == GL_TRUE;
  }

private:
  static constexpr GLuint INVALID_SHADER_ID = 0;
  GLuint shader_id_;
};

export class Program final {
public:
  Program(const Program &) = delete;
  Program &operator=(const Program &) = delete;

  Program(Program &&other) noexcept : program_id_(other.program_id_) {
    other.program_id_ = INVALID_PROGRAM_ID;
  }

  Program &operator=(Program &&other) noexcept {
    if (this != &other) {
      Delete();
      program_id_ = other.program_id_;
      other.program_id_ = INVALID_PROGRAM_ID;
    }

    return *this;
  }

  Program() : program_id_(INVALID_PROGRAM_ID) {}

  ~Program() { Delete(); }

  void Delete() {
    if (program_id_ != INVALID_PROGRAM_ID) {
      glDeleteProgram(program_id_);
      program_id_ = INVALID_PROGRAM_ID;
    }
  }

  void Bind() const {
    if (program_id_ != INVALID_PROGRAM_ID) {
      glUseProgram(program_id_);
    }
  }

  void AttachShader(Shader const &shader) {
    if (program_id_ != INVALID_PROGRAM_ID && !shader.IsNull()) {
      glAttachShader(program_id_, shader.GetID());
    }
  }
  void AttachShader(GLuint shader_id) {
    if (program_id_ != INVALID_PROGRAM_ID) {
      glAttachShader(program_id_, shader_id);
    }
  }

  bool Build(Shader const &vertex_shader, Shader const &fragment_shader) {
    CreateProgram();
    AttachShader(vertex_shader);
    AttachShader(fragment_shader);
    return Link();
  }

  bool Link() {
    if (program_id_ == INVALID_PROGRAM_ID) {
      return false;
    }
    glLinkProgram(program_id_);
    GLint result{};
    glGetProgramiv(program_id_, GL_LINK_STATUS, &result);

    GLint info_log_length{};
    glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 1) {
      std::string compiler_message(static_cast<std::size_t>(info_log_length),
                                   0);
      GLint log_length{};
      glGetProgramInfoLog(program_id_,
                          static_cast<GLsizei>(compiler_message.length()),
                          &log_length, compiler_message.data());
      compiler_message.resize(static_cast<std::size_t>(log_length));

      if (result != GL_TRUE) {
        throw std::runtime_error("Cannot link program: " + compiler_message);
      }
    }

    return result == GL_TRUE;
  }

private:
  void CreateProgram() {
    Delete();
    program_id_ = glCreateProgram();
    if (program_id_ == INVALID_PROGRAM_ID) {
      throw std::runtime_error("Failed to create program");
    }
  }

  static constexpr GLuint INVALID_PROGRAM_ID = 0;
  GLuint program_id_;
};
} // namespace runic
