#include "VideoInference.h"
#include "ConfigParser.h"


int main(int argc, char* argv[])
{
  if (argc < 2) { // requires a config file
    std::cerr << "Usage: " << argv[0] << " CONFIG" << std::endl;
  }
  std::string fname(argv[1]);
  ConfigParser config(fname);

  config.addStringParam("srcfile", "");
  config.addNumericParam("nangles", 200);
  config.addNumericParam("ncircles", 20);
  config.addNumericParam("rstep", 12);
  config.addNumericParam("lambda", 10);
  config.addNumericParam("beta", 100);
  config.addNumericParam("alpha", 0.99);
  config.addNumericParam("disp_rollup", 0);

  config.addNumericParam("start_time", 2);
  config.addNumericParam("process_fps", 20);
  config.addNumericParam("end_time", 32);

  config.parse();

  VideoInference vinf(
      (int) config.getNumericParam("nangles"),
      (int) config.getNumericParam("ncircles"),
      (float) config.getNumericParam("rstep"),
      config.getNumericParam("lambda"),
      config.getNumericParam("beta"),
      config.getNumericParam("alpha"));

  vinf.processVideo(
      config.getStringParam("srcfile"),
      (bool) config.getNumericParam("disp_rollup"),
      config.getNumericParam("start_time"),
      (int) config.getNumericParam("process_fps"),
      config.getNumericParam("end_time"));

  return 0;
}
