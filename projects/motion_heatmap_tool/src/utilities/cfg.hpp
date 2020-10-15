#pragma once

#include <array>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>

namespace vmx {

struct Config {
  struct Camera {
    std::string source;
    int64_t capture_delay_ms;
  } camera;

  struct Processor {
    std::array<float, 3> segments_rgb;
    bool show_gui;
  } processor;
};

void to_json(nlohmann::json& j, const Config::Camera& c);
void from_json(const nlohmann::json& j, Config::Camera& c);

void to_json(nlohmann::json& j, const Config::Processor& p);
void from_json(const nlohmann::json& j, Config::Processor& p);

void to_json(nlohmann::json& j, const Config& c);
void from_json(const nlohmann::json& j, Config& c);

bool ParseCfg(const std::string& content, Config& cfg);
bool ParseCfg(const std::filesystem::path& file, Config& cfg);

}  // namespace vmx
