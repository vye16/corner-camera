#include "VideoInference.h"
#include "test_utils.cpp"



int main()
{
  int testvid = 1;
  cv::Point corner, wallpt, endpt;
  int angle;

  std::string srcfile = loadTestVidInfo(testvid, 
      corner, wallpt, endpt, angle);

  int nangles = 200;
  int ncircles = 15;
  float rstep = 10;
  double lambda = 50;
  double beta = 1/(0.2*0.2);
  double alpha = 3.0/4;

  VideoInference vinf(corner, wallpt, endpt,
      nangles, ncircles, rstep, lambda, beta, alpha);

//  VideoInference vinf(nangles, ncircles,
//      rstep, lambda, beta, alpha);

  vinf.processVideo(srcfile, 0, 2, 20, 42);

//  writeCSV("amat.csv", vinf.amat());
//  writeCSV("reg.csv", vinf.reg());
//  writeCSV("xq.csv", vinf.xq());
//  writeCSV("yq.csv", vinf.yq());

  return 0;
}
