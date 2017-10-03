#ifndef INFERENCE_H
#define INFERENCE_H

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <exception>
#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "CalibrationWindow.h"


class Inference
{
  public:
    Inference(int nangles, int ncircles, float rstep,
        double lambda, double beta, double alpha);

    Inference(cv::Point corner,
        cv::Point wallpt, cv::Point endpt,
        int nangles, int ncircles, float rstep,
        double lambda, double beta, double alpha);

    void findObsRegion();
    void setObsRegion(cv::Point corner,
        cv::Point wallpt, cv::Point endpt);
    void setNumSamples(int nangles, int ncircles);
    void setNoiseLevels(double lambda, double beta);

    void setup();
    void preprocessFrame();
    void processFrame(cv::Mat& cur_disp);

    void plotObsXYLocs(std::string winname);

    /* getters */
    cv::Point2f corner() { return cv::Point2f(corner_x_, corner_y_); }
    cv::Vec2f theta_lims() { return cv::Vec2f(theta_start_, theta_end_); }
    int nangles() { return nangles_; }
    int ncircles() { return ncircles_; }
    float rstep() { return rstep_; }
    double lambda() { return lambda_; }
    double beta() { return beta_; }

    const cv::Mat& amat() { return amat_; }
    const cv::Mat& reg() { return reg_; }
    const cv::Mat& gain() { return gain_; }
    const cv::Mat& xq() { return xq_; }
    const cv::Mat& yq() { return yq_; }

  protected:
    cv::Mat frame_;
    int nrows_, ncols_;

    cv::Mat dframe_; // preprocessed type double frame
    cv::Mat backim_; // running mean of preprocessed frames
    cv::Scalar meanpx_;

    float corner_x_, corner_y_;
    float theta_start_, theta_end_;

    int downlevs_, xmin_, xmax_, ymin_, ymax_;

    int nangles_;
    int ncircles_;
    float rstep_;

    double lambda_;
    double beta_;
    double alpha_;

    cv::Mat amat_;
    cv::Mat reg_; // regularizer without beta weight
    cv::Mat gain_;
    cv::Mat xq_, yq_;
    cv::Mat tq_; // angle of sample locs from the corner

    void setAmat();
    void setRegularizer();
    void setObsXYLocs();
    void setPieSliceXYLocs();
    void updateMeanImage(int total);
};

#endif
