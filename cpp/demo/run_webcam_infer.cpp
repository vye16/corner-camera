#include "WebCamInference.h"
#include "ConfigParser.h"


int main(int argc, char* argv[])
{
  std::string cfile;
  if (argc < 2) {
    printf("No config file, using default inference parameters\n");
    std::cout << "(Usage: " << argv[0] << " CONFIG FILE)" << std::endl;
    cfile = "";
  } else {
    cfile = std::string(argv[1]);
  }
  ConfigParser config(cfile);

  config.addNumericParam("nangles", 200);
  config.addNumericParam("ncircles", 20);
  config.addNumericParam("rstep", 12);
  config.addNumericParam("lambda", 10);
  config.addNumericParam("beta", 100);
  config.addNumericParam("alpha", 0.5);
  config.addNumericParam("devid", 0);
  config.addNumericParam("disp_rollup", 0);

  config.parse();

  WebCamInference webinf(
      (int) config.getNumericParam("nangles"),
      (int) config.getNumericParam("ncircles"),
      (float) config.getNumericParam("rstep"),
      config.getNumericParam("lambda"),
      config.getNumericParam("beta"),
      config.getNumericParam("alpha"));

  webinf.processStream((int) config.getNumericParam("devid"),
      (bool) config.getNumericParam("disp_rollup"));
 
  return 0;
}
