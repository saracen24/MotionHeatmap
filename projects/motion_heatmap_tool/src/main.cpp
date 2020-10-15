#include <iostream>
#include <memory>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include "processor.hpp"
#include "utilities/cfg.hpp"
#include "utilities/cli.hpp"

using namespace std;
namespace fs = std::filesystem;
using namespace cv;
using namespace vmx;

int main(int argc, char* argv[]) {
  auto cli = ParseCli(argc, argv);
  const fs::path& config = cli["config"].as<fs::path>();

  Config cfg;
  if (!ParseCfg(config, cfg)) {
    cerr << "Configuration parsing failed." << endl;
    return -1;
  }

  VideoCapture camera(cfg.camera.source);
  unique_ptr<Processor> processor =
      make_unique<Processor>(cfg, config.parent_path());

  Mat frame;
  bool autoMode = true;

  while (camera.read(frame)) {
    processor->hypothesize(frame);

    if (cfg.processor.show_gui) {
      const char key = static_cast<char>(
          waitKeyEx(autoMode ? cfg.camera.capture_delay_ms : 0) & 0xFF);
      if (key == 'q')
        break;
      else if (key == 'm')
        autoMode = !autoMode;
    }
  }

  frame.release();
  destroyAllWindows();
  processor.reset();
  camera.release();

  return 0;
}
