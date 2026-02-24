module;

#include <iostream>

export module Runic;

namespace runic {
export void foo() { std::cout << "hello from module" << std::endl; }
} // namespace runic
