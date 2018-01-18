#include "Inference.h"
#include "test_utils.cpp"

#include "opencv2/highgui/highgui.hpp"


int main()
{
  int testvid = 0;
  cv::Point corner, wallpt, endpt;
  int angle;

  loadTestVidInfo(testvid, corner, wallpt, endpt, angle);

  int nangles = 10;
  int ncircles = 2;
  float rstep = 20;
  double lambda = 7;
  double beta = 100;
  double alpha = 0.99;

  Inference inf = Inference(corner, wallpt, endpt,
      nangles, ncircles, rstep, lambda, beta, alpha);

  inf.setup();

  /* write stuff to csv's to check against matlab */
  writeCSV("amat.csv", inf.amat());
  writeCSV("reg.csv", inf.reg());
  writeCSV("gain.csv", inf.gain());
  writeCSV("xq.csv", inf.xq());
  writeCSV("yq.csv", inf.yq());

  std::cout << "theta lims: " << inf.theta_lims() << std::endl;

  cv::Mat image = cv::imread("scratch/frame1.png");
  cv::Mat dimage;
  image.convertTo(dimage, CV_64FC3, 1.0/255.0);

  cv::Mat samples;
  cv::remap(dimage, samples, inf.xq(), inf.yq(), CV_INTER_LINEAR);

  cv::Mat samples_bgr[3];
  cv::split(samples, samples_bgr);

  cv::Mat out_bgr[3];
  out_bgr[2] = inf.gain() * samples_bgr[2];
  out_bgr[1] = inf.gain() * samples_bgr[1];
  out_bgr[0] = inf.gain() * samples_bgr[0];

  cv::Mat outvec[3];
  for (int i=0; i < 3; i++) {
    outvec[i] = cv::Mat(2, nangles+1, CV_64FC1);
    outvec[i].row(0) = (inf.gain() * samples_bgr[i]).t();
//    cv::Mat tmp = (inf.gain() * samples_bgr[i]).t();
//    tmp.copyTo(out_r.row(0));
    std::cout << outvec[i] << std::endl;
    std::cout << out_bgr[i] << std::endl;
  }

//  writeCSV("samples_r.csv", samples_bgr[2]);
//  writeCSV("samples_g.csv", samples_bgr[1]);
//  writeCSV("samples_b.csv", samples_bgr[0]);
//
//  writeCSV("out_r.csv", out_bgr[2]);
//  writeCSV("out_g.csv", out_bgr[1]);
//  writeCSV("out_b.csv", out_bgr[0]);

  return 0;
}
