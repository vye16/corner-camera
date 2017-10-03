#ifndef ROLLINGDISPLAY_H
#define ROLLINGDISPLAY_H

#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"


class RollingDisplay
{
  public:
    RollingDisplay(std::string name, bool roll_up,
        int nrows, int ncols, int buf_factor=2);
    
    RollingDisplay(std::string name, bool roll_up,
        int ncols, int buf_factor=2);

    cv::Mat nextrow();
    void update();

    std::string name() { return name_; }
    int nrows() { return nrows_; }
    int ncols() { return ncols_; }
    int startIndex() { return dstart_; }
    int currentIndex() { return drow_; }

    static const int DEFAULT_ROWS = 300;

  private:
    std::string name_;
    cv::Mat buffer_;
    cv::Mat disp_;

    bool roll_up_;
    int nrows_;
    int ncols_;
    int bufsize_;
    int dstart_; // display starting index
    int drow_; // display current row

    void wrapAround();
};

#endif
