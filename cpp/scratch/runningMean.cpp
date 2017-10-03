#include <iostream>
#include <cstdio>

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

  double dwidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  double dheight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
  printf("size: %.0f x %.0f\n", dwidth, dheight);

  cv::namedWindow("Average", CV_WINDOW_AUTOSIZE);

  cv::Mat summed = cv::Mat::zeros(dheight, dwidth, CV_64FC3);
  cv::Mat frame, temp;
  int count = 0;

  while(1) {
    bool bsuccess = cap.read(frame);
    if (!bsuccess) {
      printf("Cannot read video frame\n");
      break;
    }

    count++;
    printf("Frame %d\n", count);
    frame.convertTo(temp, CV_64FC3);
    summed += temp;

    imshow("Average", summed/255/count);

    if (cv::waitKey(30) == 27) { // esc key press
      printf("Esc key pressed by user\n");
      break;
    }
  }

  return 0;
}
