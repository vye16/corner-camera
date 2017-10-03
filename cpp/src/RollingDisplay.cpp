#include "RollingDisplay.h"

#include <algorithm>
#include <exception>


RollingDisplay::RollingDisplay(std::string name, bool roll_up,
    int ncols, int buf_factor)
{
  if (buf_factor < 1)
    throw std::runtime_error("buffer must be at least same length as display");

  name_ = name;
  roll_up_ = roll_up;
  nrows_ = DEFAULT_ROWS;
  ncols_ = ncols;

  bufsize_ = nrows_ * buf_factor;
  buffer_ = cv::Mat(bufsize_, ncols_, CV_64FC3);
  cv::namedWindow(name_, CV_WINDOW_NORMAL);

  dstart_ = 0;
  drow_ = 0;
}


RollingDisplay::RollingDisplay(std::string name, bool roll_up,
    int nrows, int ncols, int buf_factor)
{
  if (buf_factor < 1)
    throw std::runtime_error("buffer must be at least same length as display");

  name_ = name;
  roll_up_ = roll_up;
  nrows_ = nrows;
  ncols_ = ncols;

  bufsize_ = nrows_ * buf_factor;
  buffer_ = cv::Mat(bufsize_, ncols_, CV_64FC3);
  cv::namedWindow(name_, CV_WINDOW_NORMAL);

  dstart_ = 0;
  drow_ = 0;
}


cv::Mat RollingDisplay::nextrow()
{
  if (drow_ >= bufsize_) { // need to wrap around to beginning
    wrapAround();
  }
  dstart_ = std::max(0, drow_ - nrows_ + 1);
  return buffer_.row(drow_++);
}


void RollingDisplay::update()
{
  if (roll_up_) {
    cv::flip(buffer_.rowRange(dstart_, dstart_ + nrows_), disp_, 0);
  } else {
    disp_ = buffer_.rowRange(dstart_, dstart_ + nrows_);
  }
  cv::imshow(name_, disp_);
}


void RollingDisplay::wrapAround()
{ // copy the last nrows_ - 1 rows to the beginning and update drow
  int end = drow_;
  int start = end - (nrows_ - 1);
  cv::Mat last = buffer_.rowRange(start, end);
  last.copyTo(buffer_.rowRange(0, nrows_ - 1));
  drow_ = nrows_ - 1;
}
