#include <iostream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>


int main(int, char** argv)
{
  const std::string datafolder = "/Users/vickieye/Dropbox (MIT)/shadowImaging/edgeImaging/data/testvideos_Mar04/"; 
  const std::string expfolder = datafolder + "experiments/";
  const std::string resfolder = datafolder + "results/";

  const std::string name = "blue_randomwalking1";
  const std::string vidtype = ".MP4";
  const std::string srcfile = expfolder + name + vidtype;


  cv::VideoCapture cap(srcfile);

  if (!cap.isOpened()) {
    printf("Cannot read video from %s\n", srcfile.c_str());
    return -1;
  }

  cv::Mat frame;
  bool bsuccess = cap.read(frame);
  if (!bsuccess) { 
    printf("Cannot read frame\n");
    return -1;
  }
  printf("Read first frame\n");

  std::vector<int> compress_params;
  compress_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
  compress_params.push_back(9);
  imwrite("frame1.png", frame, compress_params);

  printf("First frame written to frame1.png\n");

  return 0;
}
