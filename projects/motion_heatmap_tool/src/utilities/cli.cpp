#include "cli.hpp"

#include <fmt/format.h>

#include <filesystem>
#include <iostream>
#include <string_view>

using namespace std;
namespace fs = std::filesystem;

namespace vmx {

bool CheckCli(const cxxopts::ParseResult& cli) {
  bool success = true;

  const fs::path& config = cli["config"].as<fs::path>();
  if (!fs::exists(config) || fs::is_directory(config)) {
    cerr << "Configuration is not found." << endl;
    success = false;
  }

  return success;
}

cxxopts::ParseResult ParseCli(int argc, char* argv[]) {
  try {
    string appname = fs::path(argv[0]).filename().string();
    string description = fmt::format("{} v{} - {}", string_view(PROJECT_NAME),
                                     string_view(PROJECT_VERSION),
                                     string_view(PROJECT_DESCRIPTION));
    cxxopts::Options cli(appname, description);

    cxxopts::OptionAdder add = cli.add_options();
    add("h,help", "Print help message.");
    add("c,config", "Path to configuration.", cxxopts::value<fs::path>());

    cxxopts::ParseResult result = cli.parse(argc, argv);

    if (result.count("help")) {
      clog << cli.help({""}) << endl;
      exit(EXIT_FAILURE);
    }

    if (!result.count("config")) {
      clog << cli.help({""}) << endl;
      exit(EXIT_FAILURE);
    }

    if (!CheckCli(result)) exit(EXIT_FAILURE);

    return result;
  } catch (const cxxopts::OptionException& e) {
    cerr << fmt::format("Options parsing failed: {}", e.what()) << endl;
    exit(EXIT_FAILURE);
  }
}

}  // namespace vmx
