#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"


#define PI 3.14159265


void bilinearInterpolate(const cv::Mat& src, cv::Mat& dest,
    cv::Mat xq, cv::Mat yq)
{
  assert(xq.rows == yq.rows && xq.cols == yq.cols);
  assert(xq.channels() == 1 && yq.channels() == 1);
  assert(xq.depth() == CV_32F && yq.depth() == CV_32F);
  assert(src.depth() == CV_64F);

  dest = cv::Mat(xq.rows, xq.cols, src.type());

  int nchans = src.channels();
  int ncols = src.cols;
  int numel_q = xq.rows * xq.cols;

  double *src_ptr = (double *) src.data;
  double *dest_ptr = (double *) dest.data;
  float *x_ptr = (float *) xq.data;
  float *y_ptr = (float *) yq.data;

  int x0, y0;
  float xr, yr;
  int i00, i01, i10, i11;
  for (int i = 0; i < numel_q; i++) {
    x0 = (int) x_ptr[i];
    y0 = (int) y_ptr[i];
    xr = x_ptr[i] - (float) x0;
    yr = y_ptr[i] - (float) y0;
//    printf("(x, y) = (%f, %f), (x0, y0) = (%d, %d)\n",
//        x_ptr[i], y_ptr[i], x0, y0);
    for (int k = 0; k < nchans; k++) {
//      printf("%d\n", k);
      i00 = y0*src.cols*nchans + x0*nchans + k;
      i01 = i00 + nchans;
      i10 = i00 + src.cols*nchans;
      i11 = i00 + src.cols*nchans + nchans;
//      printf("%f, %f, %f, %f\n",
//          src_ptr[i00], src_ptr[i01],
//          src_ptr[i10], src_ptr[i11]);
      dest_ptr[i*nchans + k] =
        src_ptr[i00]*(1-xr)*(1-yr) + src_ptr[i11]*xr*yr
        + src_ptr[i01]*(1-xr)*yr + src_ptr[i10]*xr*(1-yr);
    }
  }
}


void writeCSV(std::string fname, cv::Mat mat) 
{
  std::ofstream myfile;
  myfile.open(fname.c_str());
//  myfile << cv::format(mat, "CSV") << std::endl;
  myfile << cv::format(mat, cv::Formatter::FMT_CSV) << std::endl;
  myfile.close();
}


int main(int argc, char** argv)
{
  cv::Mat image = cv::imread("frame1.png");
  cv::Mat dimage;
  image.convertTo(dimage, CV_64FC3, 1.0/255.0);

  std::cout << dimage.depth() << std::endl;
  std::cout << dimage.channels() << std::endl;

  std::cout << dimage.at<cv::Vec3d>(4, 3) << std::endl;

  double *dim_ptr = (double *)dimage.data;
  std::cout << dim_ptr[4*dimage.cols*3 + 3*3 + 0] << std::endl;
  std::cout << dim_ptr[4*dimage.cols*3 + 3*3 + 1] << std::endl;
  std::cout << dim_ptr[4*dimage.cols*3 + 3*3 + 2] << std::endl;

  int nangles = 10;

  float rstep = 30;
  int ncircles = 2;

  float theta_start = 0.7401;
  float theta_end = PI;

  float corner_x = 1164.7;
  float corner_y = 109.2;

  float angle_d = (theta_end - theta_start) / (nangles-1);

  cv::Mat xq = cv::Mat::zeros(ncircles, nangles, CV_32FC1);
  cv::Mat yq = cv::Mat::zeros(ncircles, nangles, CV_32FC1);

  float *xrow;
  float *yrow;
  float rcur, acur;
  for (int ci = 0; ci < ncircles; ci++) {
    rcur += rstep;
    acur = theta_start;
    xrow = xq.ptr<float>(ci);
    yrow = yq.ptr<float>(ci);
    for (int ai = 0; ai < nangles; ai++) {
      xrow[ai] = corner_x + rcur * cos(acur);
      yrow[ai] = corner_y + rcur * sin(acur);
      acur += angle_d;
    }
  }

  if (0) {
    cv::namedWindow("Testing", CV_WINDOW_NORMAL);
    cv::Scalar color = CV_RGB(200, 220, 240);

    cv::Mat disp_image;
    cv::resize(image, disp_image, cv::Size(image.cols/2, image.rows/2));
    for (int ci = 0; ci < ncircles; ci++) {
      xrow = xq.ptr<float>(ci);
      yrow = yq.ptr<float>(ci);
      for (int ai = 0; ai < nangles-1; ai++) {
        cv::line(disp_image, cv::Point(xrow[ai]/2, yrow[ai]/2),
            cv::Point(xrow[ai+1]/2, yrow[ai+1]/2), color);
      }
    }
    cv::circle(disp_image, cv::Point(corner_x/2, corner_y/2), 3,
    CV_RGB(255, 0, 0), -1);

    cv::imshow("Testing", disp_image);
    cv::waitKey(0);
    printf("Closed window\n");
  }

  cv::Mat samples;
  cv::remap(dimage, samples, xq, yq, CV_INTER_LINEAR);
//  bilinearInterpolate(dimage, samples, xq, yq);
  std::cout << samples << std::endl;


  /* write to check later with matlab implementation */
  writeCSV("xlocs_sampled.csv", xq);
  writeCSV("ylocs_sampled.csv", yq);
  printf("x and y loc csv's written\n");

  /* separate into 3 single channel mats */
  cv::Mat samples_bgr[3];
  cv::split(samples, samples_bgr);
  writeCSV("pts_sampled_blue.csv", samples_bgr[0]);
  writeCSV("pts_sampled_green.csv", samples_bgr[1]);
  writeCSV("pts_sampled_red.csv", samples_bgr[2]);

}
