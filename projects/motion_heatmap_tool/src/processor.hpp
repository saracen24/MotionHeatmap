#pragma once

#include <aizcv/cvision/motion_heatmap.hpp>
#include <opencv2/core/mat.hpp>
#include <utilities/cfg.hpp>

namespace vmx {

class Processor {
 public:
  enum Stage : int { PREPROCESSED, SEGMENT, MASK, HEATMAP, COUNT };

  explicit Processor(const Config& cfg, const std::filesystem::path& cfgDir);
  ~Processor();

  void hypothesize(const cv::Mat& frame);

  void setOpacity(int percent);
  void setDrawStage(Stage stage);

  void draw() const;

 private:
  const Config kCfg;

  size_t mFrameNum;

  cv::Mat mCanvas;
  cv::Mat mPreprocessed;
  cv::Mat mSegment;
  cv::Mat mMask;
  cv::Mat mHeatmap;

  Stage mDrawStage;
  int mOpacity;

  std::unique_ptr<aizcv::MotionHeatmap> mMh;
};

}  // namespace vmx
