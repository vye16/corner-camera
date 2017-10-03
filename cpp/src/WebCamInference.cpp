#include "WebCamInference.h"
#include "RollingDisplay.h"


WebCamInference::WebCamInference(int nangles, int ncircles,
    float rstep, double lambda, double beta, double alpha)
  : Inference(nangles, ncircles, rstep, lambda, beta, alpha)
{
}


WebCamInference::WebCamInference(cv::Point corner,
    cv::Point wallpt, cv::Point endpt,
    int nangles, int ncircles, float rstep,
    double lambda, double beta, double alpha)
  : Inference(corner, wallpt, endpt,
      nangles, ncircles, rstep, lambda, beta, alpha)
{
}


void::WebCamInference::getNextFrame()
{
  bool ok = cap_.read(frame_);
  if (!ok) { throw std::runtime_error("capture error"); }
}


void WebCamInference::processStream(int devid, bool disp_rollup)
{ // if disp_rollup > 0, display rolls up, otherwise rolls down
  cv::destroyAllWindows();
  cap_.open(devid);
  if (!cap_.isOpened()) {
    throw std::runtime_error("Failed to connect to camera");
  }

  // give a period for the user to adjust the camera
  cv::Mat frame, dst;
  cv::namedWindow("camera adjustment", CV_WINDOW_NORMAL);
  while (1) {
    if (!cap_.read(frame)) {
      printf("Cannot read from camera\n");
      return;
    }
    cv::resize(frame, dst, cv::Size(), 0.5, 0.5, CV_INTER_AREA);
    cv::putText(dst, "Adjust camera, ESC to proceed.",
        cv::Point(30, 30), CV_FONT_HERSHEY_PLAIN, 2,
        CV_RGB(50, 50, 250), 2);
    cv::imshow("camera adjustment", dst);
    if (cv::waitKey(1) == 27) {
      printf("Camera adjusted, please do not move it anymore\n");
      break;
    }
  }
  cv::destroyWindow("camera adjustment");

  // get frame info, setup, and initialize running mean
  getNextFrame();
  nrows_ = frame_.rows;
  ncols_ = frame_.cols;
  printf("%d x %d framesize\n", nrows_, ncols_);

  findObsRegion(); // set observation region if haven't already

  setup(); // compute gain, and sample and crop locations

  // initialize the running mean 
  preprocessFrame();
  updateMeanImage(1);

  plotObsXYLocs("sample locations");

  RollingDisplay display("output", disp_rollup, nangles_);
  cv::Mat cur_disp, backdisp, camdisp;

  cv::namedWindow("webcam", CV_WINDOW_NORMAL);
  cv::namedWindow("background image", CV_WINDOW_NORMAL);

  printf("Starting inference, press esc to stop\n");
  time_t start_time = time(0);

  int total = 0;
  while (1) {
    getNextFrame();
    preprocessFrame();
    cur_disp = display.nextrow();
    processFrame(cur_disp);

    cur_disp = (cur_disp + 0.05) / 0.1;
    cv::resize(dframe_, camdisp, cv::Size(), 2, 2);
    cv::resize(backim_, backdisp, cv::Size(), 2, 2);

    cv::imshow("webcam", camdisp/255);
    cv::imshow("background image", backdisp/255);

    display.update();
    if (cv::waitKey(1) == 27) {
      printf("pressed ESC key, exiting inference\n");
      break;
    }

    updateMeanImage(total+2);
    total++;
  }
  printf("Processed %d frames\n", total);

  cap_.release();

  cv::destroyAllWindows();
}
