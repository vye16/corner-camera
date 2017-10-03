#ifndef WEBCAMINFERENCE_H
#define WEBCAMINFERENCE_H


#include "Inference.h"



class WebCamInference : public Inference
{
  public:
    WebCamInference(int nangles, int ncircles, float rstep,
        double lambda, double beta, double alpha);
    WebCamInference(cv::Point corner,
        cv::Point wallpt, cv::Point endpt,
        int nangles, int ncircles, float rstep,
        double lambda, double beta, double alpha);

    void getNextFrame();
    void processStream(int devid, bool disp_rollup);

  private:
    cv::VideoCapture cap_;
};

#endif
