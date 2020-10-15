#pragma once

#include <array>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace aizcv {

struct MotionHeatmapCfg {
  int ocv_threads;

  struct Prerpocessor {
    bool white_balance;
    double blur;
    bool convert_to_gray;
    float spatial_scale;
  } preprocessor;

  struct BgSubtractor {
    std::vector<std::string> backend_types;
    std::string backend;
    int history;
    double distance_threshold;
    bool detect_shadows;
    bool remove_shadows;
    bool gain_shadows;
    double learning_rate;
    double blur_output;
  } bg_subtractor;

  struct Accumulator {
    bool half_precision;
    double alpha;
    double beta;
    double gamma;
  } accumulator;
};

void to_json(nlohmann::json& j, const MotionHeatmapCfg::Prerpocessor& p);
void from_json(const nlohmann::json& j, MotionHeatmapCfg::Prerpocessor& p);

void to_json(nlohmann::json& j, const MotionHeatmapCfg ::BgSubtractor& b);
void from_json(const nlohmann::json& j, MotionHeatmapCfg ::BgSubtractor& b);

void to_json(nlohmann::json& j, const MotionHeatmapCfg ::Accumulator& a);
void from_json(const nlohmann::json& j, MotionHeatmapCfg ::Accumulator& a);

void to_json(nlohmann::json& j, const MotionHeatmapCfg& c);
void from_json(const nlohmann::json& j, MotionHeatmapCfg& c);

bool ParseCfg(const std::string& content, MotionHeatmapCfg& cfg);
bool ParseCfg(const std::filesystem::path& file, MotionHeatmapCfg& cfg);

}  // namespace aizcv
