#include "PointGreyInference.h"
#include "test_utils.cpp"


int main()
{

  int nangles = 100;
  int ncircles = 10;
  float rstep = 20;
  double lambda = 7;
  double beta = 100;
  double alpha = 3.0/4;

  PointGreyInference pginf(nangles, ncircles,
      rstep, lambda, beta, alpha);

  pginf.processStream(0);
  
  return 0;
}
