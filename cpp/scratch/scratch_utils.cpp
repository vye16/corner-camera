#include <chrono>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"


typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::milliseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()


bool loadFrameAndInfo(int test_vid, bool color,
    cv::Mat& img, cv::Point& corner, int& wall_angle)
{
  std::string expname, vidname, vidtype;
  int f_down;

  switch (test_vid) {
    case 0:
      expname = "testvideos_Jan22";
      vidname = "blue_noartificiallight";
      vidtype = ".MOV";
      f_down = 1;
      corner = cv::Point(612/f_down, 146/f_down);
      wall_angle = 148;
      break;
    case 1:
      expname = "testvideos_Mar04";
      vidname = "blue_randomwalking1";
      vidtype = ".MP4";
      f_down = 2;
      corner = cv::Point(1166/f_down, 104/f_down);
      wall_angle = 42;
      break;
    case 2:
      expname = "testvideos_Mar03";
      vidname = "blue_randomwalking2_loc2";
      vidtype = ".MP4";
      f_down = 2;
      corner = cv::Point(848/f_down, 119/f_down);
      wall_angle = 118;
      break;
    case 3:
      expname = "stereoDoor_Feb20";
      vidname = "red12_walking";
      vidtype = ".MP4";
      f_down = 2;
      corner = cv::Point(542/f_down, 263/f_down);
      wall_angle = 191;
      break;
    default:
      return 0; /* lol only have these four test cases */
  }

  std::string datadir = "/Users/vickieye/Dropbox (MIT)/shadowImaging/edgeImaging/data/";
  std::string expdir = datadir + expname + "/experiments/";
  std::string fname = expdir + vidname + vidtype;

  printf("loading from video %s\n", fname.c_str());
  printf("corner [%d, %d]\n", corner.x, corner.y);
  printf("wall angle %d\n", wall_angle);

  cv::VideoCapture cap(fname);
  if (!cap.isOpened()) {
    printf("cannot read video\n");
    return 0;
  }

  if (!cap.read(img)) {
    printf("cannot read first frame\n");
    return 0;
  }

  if (!color) {
    cv::cvtColor(img, img, CV_BGR2GRAY);
  }
  /* resize for ease of use and display */
  cv::resize(img, img, cv::Size(
        img.cols/f_down, img.rows/f_down));
  return 1;
}


void rotateIm(const cv::Mat& img1, cv::Mat& img2, double angle)
{
  cv::Point center = cv::Point(img1.cols/2, img1.rows/2);
  cv::Mat rotmat = cv::getRotationMatrix2D(center, angle, 1.0);
  cv::warpAffine(img1, img2, rotmat, img1.size());
}


void drawHoughLines(std::vector<cv::Vec2f> lines,
    cv::Mat& img, int topn=5)
{
  for (int i = 0; i < fmin(topn, lines.size()); i++) {
    float r = lines[i][0], theta = lines[i][1];
    double a = cos(theta), b = sin(theta);
    double x0 = r*a, y0 = r*b;
    
    cv::Point pt1, pt2;
    pt1.x = round(x0 - 1000*b);
    pt1.y = round(y0 + 1000*a);
    pt2.x = round(x0 + 1000*b);
    pt2.y = round(y0 - 1000*a);
    cv::line(img, pt1, pt2, cv::Scalar(255,255,255), 2, CV_AA);
  }
}

void drawHoughLinesP(std::vector<cv::Vec4i> lines,
    cv::Mat& img, int topn=5)
{
  for (int i = 0; i < fmin(topn, lines.size()); i++) {
    cv::Vec4i l = lines[i];
    cv::line(img, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]),
        cv::Scalar(255,255,255), 2, CV_AA);
  }
}

