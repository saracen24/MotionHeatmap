#pragma once

#include "cxxopts.hpp"

namespace vmx {

cxxopts::ParseResult ParseCli(int argc, char* argv[]);

}  // namespace vmx
