#ifndef POINTGREYINFERENCE_H
#define POINTGREYINFERENCE_H


#include "FlyCapture2.h"
#include "Inference.h"


class PointGreyInference : public Inference
{
  public:
    PointGreyInference(int nangles, int ncirlces, float rstep,
        double lambda, double beta, double alpha);
    PointGreyInference(cv::Point corner,
        cv::Point wallpt, cv::Point endpt,
        int nangles, int ncirlces, float rstep,
        double lambda, double beta, double alpha);

    void getNextFrame(FlyCapture2::Camera *cam);
    void processStream(bool disp_rollup);

    void setProperty(FlyCapture2::Camera *cam,
        FlyCapture2::PropertyType propType, float ss);
    void setExposure(FlyCapture2::Camera *cam, float ss);
    void setGain(FlyCapture2::Camera *cam, float ss);
    void setFPS(FlyCapture2::Camera *cam, float ss);
    void autoAdjustProperty(FlyCapture2::Camera *cam,
        FlyCapture2::PropertyType propType);
    void disableAuto(FlyCapture2::Camera *cam,
        FlyCapture2::PropertyType propType);

  private:
    int ndisp_rows_;
    int ndisp_cols_;
};


#endif
