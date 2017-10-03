#include <iostream>

#include "opencv2/highgui.hpp"

using namespace cv;
using namespace std;


int main(int argc, char* argv[])
{
  int device_id = 0;
  VideoCapture cap(device_id); // open the video camera 0
  
  if (!cap.isOpened()) {
    cout << "Cannot open video cam" << endl;
    return -1;
  }

  double dwidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  double dheight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "frame size: " << dwidth << " x " << dheight << endl;

  namedWindow("Video", CV_WINDOW_AUTOSIZE);

  while(1) {
    Mat frame;
    cout << "exposure " << cap.get(CV_CAP_PROP_EXPOSURE);
    cout << ", brightness " << cap.get(CV_CAP_PROP_BRIGHTNESS);
    cout << ", contrast " << cap.get(CV_CAP_PROP_CONTRAST);
    cout << ", saturation " << cap.get(CV_CAP_PROP_SATURATION);
    cout << ", gain " << cap.get(CV_CAP_PROP_GAIN) << endl;

    bool bsuccess = cap.read(frame); // read a new frame
    if (!bsuccess) {
      cout << "cannot read frame from video stream" << endl;
      break;
    }

    imshow("Video", frame); // show frame in window
    if (waitKey(30) == 27) { // wait for esc keypress for 30ms to exit
      cout << "esc key pressed by user" << endl;
      break;
    }
  }
  return 0;
}
