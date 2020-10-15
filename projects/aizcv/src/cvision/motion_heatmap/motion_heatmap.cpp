#include <aizcv/cvision/motion_heatmap.hpp>
#include <cmath>
#include <iostream>
#include <opencv2/bgsegm.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/xphoto.hpp>

#include "motion_heatmap_cfg.hpp"

using namespace std;
namespace fs = std::filesystem;
using namespace cv;

namespace aizcv {

class MotionHeatmap::MotionHeatmapImpl {
 public:
  MotionHeatmapImpl() = default;
  ~MotionHeatmapImpl() = default;

  bool initialize(const char *cfg) {
    if (!ParseCfg(fs::path(cfg), mCfg)) {
      cerr << "Configuration parsing failed." << endl;
      return false;
    }

    setNumThreads(mCfg.ocv_threads);
    mWb = xphoto::createSimpleWB();

    if (mCfg.bg_subtractor.backend == "MOG2")
      mBgs = createBackgroundSubtractorMOG2(
          mCfg.bg_subtractor.history, mCfg.bg_subtractor.distance_threshold,
          mCfg.bg_subtractor.detect_shadows);
    else if (mCfg.bg_subtractor.backend == "KNN")
      mBgs = createBackgroundSubtractorKNN(
          mCfg.bg_subtractor.history, mCfg.bg_subtractor.distance_threshold,
          mCfg.bg_subtractor.detect_shadows);

    return true;
  }

  void preprocess(const cv::Mat &src, cv::Mat &dst) {
    dst = src.clone();

    if (mCfg.preprocessor.white_balance) mWb->balanceWhite(dst, dst);

    if (isgreater(mCfg.preprocessor.blur, 0.))
      GaussianBlur(dst, dst, Size(), mCfg.preprocessor.blur);

    if (mCfg.preprocessor.convert_to_gray) cvtColor(dst, dst, COLOR_BGR2GRAY);

    if (isgreater(mCfg.preprocessor.spatial_scale, 0.f) &&
        isless(mCfg.preprocessor.spatial_scale, 1.f)) {
      mOriginalSize = dst.size();
      resize(dst, dst,
             Size(cvRound(dst.cols * mCfg.preprocessor.spatial_scale),
                  cvRound(dst.rows * mCfg.preprocessor.spatial_scale)));
    }
  }

  void segment(const cv::Mat &src, cv::Mat &dst) {
    dst = src;

    mBgs->apply(dst, dst, mCfg.bg_subtractor.learning_rate);

    if (mCfg.bg_subtractor.remove_shadows)
      threshold(dst, dst, 127, 255, THRESH_BINARY);
    else if (mCfg.bg_subtractor.gain_shadows)
      threshold(dst, dst, 0, 255, THRESH_BINARY);

    if (isgreater(mCfg.bg_subtractor.blur_output, 0.))
      GaussianBlur(dst, dst, Size(), mCfg.bg_subtractor.blur_output);
  }

  void accumulate(const cv::Mat &src, cv::Mat &dst) {
    dst = src;

    if (mAccumulator.empty()) {
      mAccumulator = mCfg.accumulator.half_precision
                         ? Mat::zeros(dst.size(), CV_32FC(dst.channels()))
                         : Mat::zeros(dst.size(), CV_64FC(dst.channels()));
    }

    dst.convertTo(dst, mAccumulator.type());
    normalize(dst, dst, 0., 1., NORM_MINMAX);
    addWeighted(mAccumulator, mCfg.accumulator.alpha, dst,
                mCfg.accumulator.beta, mCfg.accumulator.gamma, mAccumulator);

    dst = mAccumulator.clone();
    if (isgreater(mCfg.preprocessor.spatial_scale, 0.f) &&
        isless(mCfg.preprocessor.spatial_scale, 1.f))
      resize(dst, dst, mOriginalSize);
    normalize(dst, dst, 0., 1., NORM_MINMAX);
  }

 private:
  MotionHeatmapCfg mCfg;
  Size mOriginalSize;
  Ptr<xphoto::WhiteBalancer> mWb;
  Ptr<BackgroundSubtractor> mBgs;
  Mat mAccumulator;
};

MotionHeatmap::MotionHeatmap() : mPimpl(new MotionHeatmapImpl) {}

MotionHeatmap::~MotionHeatmap() { delete mPimpl; }

bool MotionHeatmap::initialize(const char *cfg) {
  return mPimpl->initialize(cfg);
}

void MotionHeatmap::preprocess(const Mat &src, Mat &dst) {
  mPimpl->preprocess(src, dst);
}

void MotionHeatmap::segment(const Mat &src, Mat &dst) {
  mPimpl->segment(src, dst);
}

void MotionHeatmap::accumulate(const Mat &src, Mat &dst) {
  mPimpl->accumulate(src, dst);
}

bool MotionHeatmap::deinitialize() { return true; }

}  // namespace aizcv
