#ifndef PARAMETERS_H
#define PARAMETERS_H


#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <algorithm>
#include <exception>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>



class CalibrationWindow 
{
  public:
    CalibrationWindow(std::string win_name, const cv::Mat& cal_image);
    static const int npts_needed = 3;

    static void onClick(int event, int x, int y,
        int flags, void* args);
    void handleNewPoint(int x, int y);
    void show();

    std::string name() { return name_; }
    const cv::Mat& displayImage() { return disp_image_; }
    cv::Point corner();
    cv::Point wallPoint();
    cv::Point endPoint();

    /* display things */
    cv::Scalar pt_color;
    int pt_size;

    cv::Scalar txt_color;
    int txt_font;
    double txt_scale;
    int txt_thickness;
    int txt_padding;
    int txt_height;

  private:

    std::string name_;
    cv::Mat disp_image_;
    float resize_fx_, resize_fy_;
    std::vector<cv::Point> clicked_pts_;
};

#endif
