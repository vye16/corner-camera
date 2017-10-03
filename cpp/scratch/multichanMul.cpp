#include <iostream>

#include "opencv2/core.hpp"


int main()
{
  cv::Mat A(4, 4, CV_64FC3);
  cv::Mat B(4, 4, CV_64FC3);

  cv::Mat mean = cv::Mat::zeros(1, 1, CV_64FC1);
  cv::Mat std = cv::Mat::ones(1, 1, CV_64FC1);
  cv::randu(A, mean, std);
  cv::randu(B, mean, std);

  std::cout << "A, type " << A.type() << std::endl;
  std::cout << A << std::endl;
  std::cout << "B, type " << B.type() << std::endl;
  std::cout << B << std::endl;

  cv::Mat C = A * B;
  std::cout << "C, type " << C.type() << std::endl;

  cv::Mat achans[3];
  cv::split(A, achans);

  cv::Mat bchans[3];
  cv::split(B, bchans);

  cv::Mat cchans[3];
  cv::split(C, cchans);

  for (int i = 0; i < 3; i++) {
    std::cout << (achans[1] * bchans[1] - cchans[1]) << std::endl;
  }

  return 0;
}
