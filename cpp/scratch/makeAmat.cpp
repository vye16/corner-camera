#include <iostream>
#include <string>

#include <opencv2/core.hpp>


int main(int argc, char** argv)
{
  int nsamples, ncircles;
  if (argc < 3) { // default values
    nsamples = 10;
    ncircles = 2;
  } else {
    nsamples = std::stoi(argv[1]);
    ncircles = std::stoi(argv[2]);
  }

  int ydim = nsamples;
  int xdim = nsamples + 1;

  cv::Mat amat = cv::Mat::zeros(ydim * ncircles, xdim, CV_64FC1);

  int ridx;
  double* row;
  for (int circle = 0; circle < ncircles; circle++) {
    for (int i = 0; i < ydim; i++) {
      ridx = circle * ydim + i;
      row = amat.ptr<double>(ridx); // pointer to this row
      for (int j = 0; j <= i+1 && j < xdim; j++) { 
        row[j] = 1;
      }
    }
  }

  std::cout << amat << std::endl;

  cv::Mat bmat = cv::Mat::zeros(xdim-1, xdim, CV_64FC1);

  //double *row;
  for (int i = 1; i < bmat.rows; i++) {
    row = bmat.ptr<double>(i);
    for (int j = 1; j < bmat.cols-1; j++) {
      row[j] = 1;
      row[j+1] = -1;
    }
  }
  
  std::cout << bmat << std::endl;
}
