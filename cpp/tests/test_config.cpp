#include "ConfigParser.h"


int main()
{
  std::string fname = "config_test.conf";
  ConfigParser config(fname);
  config.addNumericParam("lambda", 10);
  config.addNumericParam("beta", 1/(0.2*0.2));
  config.addStringParam("outfile", "test_out.txt");

  config.parse();

  printf("\nConfig contents:\n");
  printf("- lambda %f\n", config.getNumericParam("lambda"));
  printf("- beta %f\n", config.getNumericParam("beta"));
  printf("- outfile %s\n", config.getStringParam("outfile").c_str());
}
