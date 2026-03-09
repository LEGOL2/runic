module;

#include <glad/glad.h>
#include <stdexcept>
#include <string>
#include <vector>

export module Runic.Program;

import Runic.Utils;

namespace runic {
export class Shader final {
public:
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

  bool Compile(std::string &shader_file, GLenum shader_type) {
    Delete();
    shader_id_ = glCreateShader(shader_type);
    auto source_code = runic::utils::LoadSource(shader_file);
    auto *src = source_code.c_str();
    glShaderSource(shader_id_, 1, &src, nullptr);
    glCompileShader(shader_id_);

    GLint result{};
    glGetShaderiv(shader_id_, GL_COMPILE_STATUS, &result);

    GLint info_log_length{};
    glGetShaderiv(shader_id_, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 1) {
      std::string compiler_message(info_log_length, 0);
      glGetShaderInfoLog(shader_id_, compiler_message.length(), nullptr,
                         compiler_message.data());
      throw std::runtime_error("Cannot compile shader: " + compiler_message);
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
  static constexpr GLuint INVALID_SHADER_ID = 0xFFFFFFFF;
  GLuint shader_id_;
};

export class Program final {
public:
  Program() : program_id_(INVALID_PROGRAM_ID) {}
  ~Program() { Delete(); }

  void Delete() {
    if (program_id_ != INVALID_PROGRAM_ID) {
      glDeleteProgram(program_id_);
      program_id_ = INVALID_PROGRAM_ID;
    }
  }

  void Bind() const { glUseProgram(program_id_); }

  void CreateProgram() {
    Delete();
    program_id_ = glCreateProgram();
  }

  void AttachShader(Shader const &shader) {
    glAttachShader(program_id_, shader.GetID());
  }
  void AttachShader(GLuint shader_id) {
    glAttachShader(program_id_, shader_id);
  }

private:
  static constexpr GLuint INVALID_PROGRAM_ID = 0xFFFFFFFF;
  GLuint program_id_;
};
} // namespace runic
