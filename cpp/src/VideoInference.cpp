#include "VideoInference.h"
#include "RollingDisplay.h"


VideoInference::VideoInference( int nangles, int ncircles,
    float rstep, double lambda, double beta, double alpha)
  : Inference(nangles, ncircles, rstep, lambda, beta, alpha)
{
}


VideoInference::VideoInference(cv::Point corner,
    cv::Point wallpt, cv::Point endpt,
    int nangles, int ncircles, float rstep,
    double lambda, double beta, double alpha)
  : Inference(corner, wallpt, endpt,
      nangles, ncircles, rstep, lambda, beta, alpha)
{
}


void VideoInference::getNextFrame()
{
  bool ok = cap_.read(frame_);
  if (!ok) { throw std::runtime_error("cannot read frame"); }
}


void VideoInference::skipFrames(int n)
{
  bool ok = 1;
  for (int i = 0; i < n && ok; i++) {
    ok = cap_.grab();
  }
  if (!ok) { throw std::runtime_error("cannot skip frames"); }
}


void VideoInference::processVideo(std::string vidname,
    bool disp_rollup, double start_time,
    int process_fps, double end_time)
{ // if disp_rollup > 0, display rolls up, otherwise rolls down
  cv::destroyAllWindows();

  cap_.open(vidname);
  if (!cap_.isOpened()) {
    throw std::runtime_error("cannot open video");
  }

  int vid_fps = round(cap_.get(CV_CAP_PROP_FPS));
  int start_f = start_time * vid_fps;
  int step = round(vid_fps / process_fps);
  int end_f = round(end_time * vid_fps);
  end_f = std::min(end_f, (int) cap_.get(CV_CAP_PROP_FRAME_COUNT));

  printf("start %d, step %d, end %d\n", start_f, step, end_f);

  // get frame info, setup, and initialize mean
  skipFrames(start_f-step-1); // skip to a step before target frames
  getNextFrame();
  nrows_ = frame_.rows;
  ncols_ = frame_.cols;
  printf("%d x %d framesize\n", nrows_, ncols_);

  findObsRegion(); // set observation region if haven't already

  setup(); // compute gain, and sample and crop locations

  // initialize running mean
  preprocessFrame();
  updateMeanImage(1);

  plotObsXYLocs("sample locations");

  // init display
  RollingDisplay display("output", disp_rollup, nangles_);
  cv::Mat cur_disp;

  cv::namedWindow("input", CV_WINDOW_NORMAL);
  cv::namedWindow("background image", CV_WINDOW_NORMAL);

  int i;
  int nframes = (end_f - start_f)/step + 1;

  printf("Starting inference, press esc to break\n");
  time_t start_t = time(0);

  for (i = 0; i < nframes; i++) {
    skipFrames(step-1);
    getNextFrame();
    if (frame_.rows == 0 || frame_.cols == 0) {
      break;
    }
    preprocessFrame();

    cur_disp = display.nextrow();
    processFrame(cur_disp);

    cur_disp = (cur_disp + 0.05) / 0.1;

    cv::imshow("input", dframe_/255);
    cv::imshow("background image", backim_/255);

    display.update();
    if (cv::waitKey(1) == 27) {
      printf("pressed ESC key, exiting inference\n");
      break;
    }
    updateMeanImage(i+2);
  }

  time_t end_t = time(0);
  double timing = (double) std::difftime(end_t, start_t);
  cv::waitKey(0);

  double fps = i/timing;
  printf("%f seconds for %d rows, %f frames per second\n",
      timing, i, fps);

  cv::destroyAllWindows();
}

