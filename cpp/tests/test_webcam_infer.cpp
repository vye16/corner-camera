#include "WebCamInference.h"
#include "test_utils.cpp"


int main()
{

  cv::Point corner = cv::Point(216, 104);
  cv::Point wallpt = cv::Point(330, 230);
  cv::Point endpt = cv::Point(100, 100);
  int nangles = 100;
  int ncircles = 10;
  float rstep = 5;
  double lambda = 7;
  double beta = 100;
  double alpha = 3.0/4;

  WebCamInference webinf(nangles, ncircles,
      rstep, lambda, beta, alpha);

  int devid = 0;
  webinf.processStream(devid, 0);
 
  return 0;
}
