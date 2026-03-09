module;

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

export module Runic.Utils;

namespace runic::utils {
export std::string LoadSource(const std::string &file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file: " + file_path);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}
} // namespace runic
