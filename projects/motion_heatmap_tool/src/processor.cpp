#include "processor.hpp"

#include <cmath>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
namespace fs = std::filesystem;
using namespace cv;
using namespace aizcv;

namespace vmx {

static const String kWindowCaption =
    format("[%s] %s", string_view(PROJECT_NAME).data(), "Canvas");
static const String kStageTrackbarCaption =
    format("%s [ %s | %s | %s | %s ]", "Stage", "Preprocessed", "Segment",
           "Mask", "Heatmap");
static const String kOpacityTrackbarCaption = "Opacity";

void Processor::draw() const {
  if (!kCfg.processor.show_gui) return;

  Mat tmpMask = mMask.clone();
  Mat tmpHeatmap = mHeatmap.clone();

  if (mDrawStage == Stage::HEATMAP || mDrawStage == Stage::MASK) {
    convertScaleAbs(tmpHeatmap, tmpHeatmap, 255.);
    cvtColor(tmpHeatmap, tmpHeatmap, COLOR_GRAY2BGR);

    const auto& rgb = kCfg.processor.segments_rgb;
    multiply(tmpHeatmap, CV_RGB(rgb[0], rgb[1], rgb[2]), tmpHeatmap);

    if (mDrawStage == Stage::HEATMAP)
      addWeighted(mCanvas, 1., tmpHeatmap, mOpacity / 100.f, 0., tmpHeatmap);
    else if (mDrawStage == Stage::MASK) {
      threshold(tmpHeatmap, tmpMask, 0., 255., THRESH_BINARY);
      addWeighted(mCanvas, 1., tmpMask, mOpacity / 100.f, 0., tmpMask);
    }
  }

  if (mDrawStage == Stage::HEATMAP)
    imshow(kWindowCaption, tmpHeatmap);
  else if (mDrawStage == Stage::MASK)
    imshow(kWindowCaption, tmpMask);
  else if (mDrawStage == Stage::SEGMENT)
    imshow(kWindowCaption, mSegment);
  else if (mDrawStage == Stage::PREPROCESSED)
    imshow(kWindowCaption, mPreprocessed);
}

static void onStageTrackbarChange(int pos, void* userdata) {
  setTrackbarPos(kStageTrackbarCaption, kWindowCaption, pos);

  Processor* const p = static_cast<Processor* const>(userdata);
  p->setDrawStage(static_cast<Processor::Stage>(pos));
  p->draw();
}

static void onOpacityTrackbarChange(int pos, void* userdata) {
  setTrackbarPos(kOpacityTrackbarCaption, kWindowCaption, pos);

  Processor* const p = static_cast<Processor* const>(userdata);
  p->setOpacity(pos);
  p->draw();
}

Processor::Processor(const Config& cfg, const fs::path& cfgDir)
    : kCfg(cfg),
      mDrawStage(Stage::HEATMAP),
      mOpacity(100),
      mMh(make_unique<MotionHeatmap>()) {
  if (kCfg.processor.show_gui) {
    namedWindow(kWindowCaption,
                WINDOW_NORMAL | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);

    createTrackbar(kStageTrackbarCaption, kWindowCaption,
                   reinterpret_cast<int*>(&mDrawStage), Stage::COUNT - 1,
                   onStageTrackbarChange, this);

    createTrackbar(kOpacityTrackbarCaption, kWindowCaption, &mOpacity, 100,
                   onOpacityTrackbarChange, this);
  }

  fs::path mhf = cfgDir;
  mhf.append("motion_heatmap.json");
  mMh->initialize(mhf.string().c_str());
}

Processor::~Processor() { mMh->deinitialize(); }

void Processor::hypothesize(const Mat& frame) {
  mCanvas = frame.clone();
  mFrameNum++;

  mMh->preprocess(mCanvas, mPreprocessed);
  mMh->segment(mPreprocessed, mSegment);
  mMh->accumulate(mSegment, mHeatmap);

  draw();
}

void Processor::setOpacity(int percent) { mOpacity = percent; }

void Processor::setDrawStage(Stage stage) { mDrawStage = stage; }

}  // namespace vmx
