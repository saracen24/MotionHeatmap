#pragma once

#include <aizcv/visibility.hpp>

namespace cv {
class Mat;
}

namespace aizcv {

class AIZCV_API MotionHeatmap {
 public:
  MotionHeatmap();
  ~MotionHeatmap();

  bool initialize(const char* cfg);
  void preprocess(const cv::Mat& src, cv::Mat& dst);
  void segment(const cv::Mat& src, cv::Mat& dst);
  void accumulate(const cv::Mat& src, cv::Mat& dst);
  bool deinitialize();

 private:
  class MotionHeatmapImpl;
  MotionHeatmapImpl* mPimpl;
};

}  // namespace aizcv
