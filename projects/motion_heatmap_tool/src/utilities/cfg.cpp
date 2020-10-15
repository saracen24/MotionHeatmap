#include "cfg.hpp"

#include <fstream>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

namespace vmx {

void to_json(json& j, const Config::Camera& c) {
  j = json{{"source", c.source}, {"capture_delay_ms", c.capture_delay_ms}};
}

void from_json(const json& j, Config::Camera& c) {
  j.at("source").get_to(c.source);
  j.at("capture_delay_ms").get_to(c.capture_delay_ms);
}

void to_json(json& j, const Config::Processor& p) {
  j = json{{"segments_rgb", p.segments_rgb}, {"show_gui", p.show_gui}};
}

void from_json(const json& j, Config::Processor& p) {
  j.at("segments_rgb").get_to(p.segments_rgb);
  j.at("show_gui").get_to(p.show_gui);
}

void to_json(json& j, const Config& c) {
  j = json{{"camera", c.camera}, {"processor", c.processor}};
}

void from_json(const json& j, Config& c) {
  j.at("camera").get_to(c.camera);
  j.at("processor").get_to(c.processor);
}

bool ParseCfg(const string& content, Config& cfg) {
  if (content.empty()) return false;

  try {
    json::parse(content).get_to(cfg);
  } catch (const json::exception& e) {
    cerr << e.what() << endl;
    return false;
  }

  return true;
}

bool ParseCfg(const fs::path& file, Config& cfg) {
  if (!fs::exists(file)) return false;

  ifstream stream(file);
  if (!stream) return false;
  string content((istreambuf_iterator<char>(stream)),
                 istreambuf_iterator<char>());
  stream.close();

  return ParseCfg(content, cfg);
}

}  // namespace vmx
