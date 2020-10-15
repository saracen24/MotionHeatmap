#include "motion_heatmap_cfg.hpp"

#include <fstream>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

namespace aizcv {

void to_json(json& j, const MotionHeatmapCfg::Prerpocessor& p) {
  j = json{{"white_balance", p.white_balance},
           {"blur", p.blur},
           {"convert_to_gray", p.convert_to_gray},
           {"spatial_scale", p.spatial_scale}};
}

void from_json(const json& j, MotionHeatmapCfg::Prerpocessor& p) {
  j.at("white_balance").get_to(p.white_balance);
  j.at("blur").get_to(p.blur);
  j.at("convert_to_gray").get_to(p.convert_to_gray);
  j.at("spatial_scale").get_to(p.spatial_scale);
}

void to_json(json& j, const MotionHeatmapCfg::BgSubtractor& b) {
  j = json{{"backend_types", b.backend_types},
           {"backend", b.backend},
           {"history", b.history},
           {"distance_threshold", b.distance_threshold},
           {"detect_shadows", b.detect_shadows},
           {"remove_shadows", b.remove_shadows},
           {"gain_shadows", b.gain_shadows},
           {"learning_rate", b.learning_rate},
           {"blur_output", b.blur_output}};
}

void from_json(const json& j, MotionHeatmapCfg::BgSubtractor& b) {
  j.at("backend_types").get_to(b.backend_types);
  j.at("backend").get_to(b.backend);
  j.at("history").get_to(b.history);
  j.at("distance_threshold").get_to(b.distance_threshold);
  j.at("detect_shadows").get_to(b.detect_shadows);
  j.at("remove_shadows").get_to(b.remove_shadows);
  j.at("gain_shadows").get_to(b.gain_shadows);
  j.at("learning_rate").get_to(b.learning_rate);
  j.at("blur_output").get_to(b.blur_output);
}

void to_json(json& j, const MotionHeatmapCfg::Accumulator& a) {
  j = json{{"half_precision", a.half_precision},
           {"alpha", a.alpha},
           {"beta", a.beta},
           {"gamma", a.gamma}};
}

void from_json(const json& j, MotionHeatmapCfg::Accumulator& a) {
  j.at("half_precision").get_to(a.half_precision);
  j.at("alpha").get_to(a.alpha);
  j.at("beta").get_to(a.beta);
  j.at("gamma").get_to(a.gamma);
}

void to_json(json& j, const MotionHeatmapCfg& c) {
  j = json{{"ocv_threads", c.ocv_threads},
           {"preprocessor", c.preprocessor},
           {"bg_subtractor", c.bg_subtractor},
           {"accumulator", c.accumulator}};
}

void from_json(const json& j, MotionHeatmapCfg& c) {
  j.at("ocv_threads").get_to(c.ocv_threads);
  j.at("preprocessor").get_to(c.preprocessor);
  j.at("bg_subtractor").get_to(c.bg_subtractor);
  j.at("accumulator").get_to(c.accumulator);
}

bool ParseCfg(const string& content, MotionHeatmapCfg& cfg) {
  if (content.empty()) return false;

  try {
    json::parse(content).get_to(cfg);
  } catch (const json::exception& e) {
    cerr << e.what() << endl;
    return false;
  }

  return true;
}

bool ParseCfg(const fs::path& file, MotionHeatmapCfg& cfg) {
  if (!fs::exists(file)) return false;

  ifstream stream(file);
  if (!stream) return false;
  string content((istreambuf_iterator<char>(stream)),
                 istreambuf_iterator<char>());
  stream.close();

  return ParseCfg(content, cfg);
}

}  // namespace aizcv
