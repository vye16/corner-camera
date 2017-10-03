#ifndef VIDEOINFERENCE_H
#define VIDEOINFERENCE_H


#include "Inference.h"


class VideoInference : public Inference
{
  public:
    VideoInference(int nangles, int ncircles, float rstep,
        double lambda, double beta, double alpha);
    VideoInference(cv::Point corner,
        cv::Point wallpt, cv::Point endpt,
        int nangles, int ncircles, float rstep,
        double lambda, double beta, double alpha);

    void getNextFrame();
    void skipFrames(int n);
    // if disp_rollup > 0, roll up, otherwise roll down
    void processVideo(std::string vidname, bool disp_rollup,
        double start_time, int process_fps, double end_time);

  private:
    cv::VideoCapture cap_;
};

#endif
