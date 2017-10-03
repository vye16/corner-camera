#include "CalibrationWindow.h"


int main()
{
  const std::string datafolder = "/Users/vickieye/Dropbox (MIT)/shadowImaging/edgeImaging/data/testvideos_Mar04/"; 
//  const std::string expfolder = datafolder + "experiments/";
  const std::string expfolder = "/home/vickieye/Downloads/";

  const std::string name = "blue_randomwalking1";
  const std::string vidtype = ".MP4";
  const std::string srcfile = expfolder + name + vidtype;

  cv::VideoCapture cap(srcfile);

  if (!cap.isOpened()) {
    printf("Cannot read video from %s\n", srcfile.c_str());
    return -1;
  }

  cv::Mat frame;
  bool ok = cap.read(frame);
  if (!ok) {
    printf("Cannot read in frame\n");
    return -1;
  }

  CalibrationWindow win("test", frame);
  win.show();

  std::cout << "corner: " << win.corner() << std::endl;
  std::cout << "wallpt: " << win.wallPoint() << std::endl;

  return 0;
}
