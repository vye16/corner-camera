#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <exception>


std::string loadTestVidInfo(int testvid, cv::Point& corner,
    cv::Point& wallpt, cv::Point& endpt, int& angle)
{
  std::string expname, vidname, vidtype;

  switch (testvid) {
    case 0:
      expname = "testvideos_Jan22";
      vidname = "redblue_dark";
      vidtype = ".MOV";
      corner = cv::Point(612, 146);
      wallpt = cv::Point(344, 316);
      endpt = cv::Point(729, 240);
      angle = 148;
      break;
    case 1:
      expname = "testvideos_Mar04";
      vidname = "blue_randomwalking1";
      vidtype = ".MP4";
      corner = cv::Point(1166, 104);
      wallpt = cv::Point(1330, 259);
      endpt = cv::Point(955, 115);
      angle = 42;
      break;
    case 2:
      expname = "testvideos_Mar03";
      vidname = "blue_randomwalking2_loc2";
      vidtype = ".MP4";
      corner = cv::Point(848, 119);
      wallpt = cv::Point(662, 462);
      endpt = cv::Point(1171, 172);
      angle = 118;
      break;
    case 3:
      expname = "stereoDoor_Feb20";
      vidname = "red12_walking";
      vidtype = ".MP4";
      corner = cv::Point(542, 263);
      wallpt = cv::Point(400, 235);
      endpt = cv::Point(433, 430);
      angle = 191;
      break;
    default:
      std::runtime_error("choose another test video");
  }

  std::string datadir = "/Users/vickieye/Dropbox (MIT)/shadowImaging/edgeImaging/data/";
  std::string expdir = datadir + expname + "/experiments/";
  std::string srcfile = expdir + vidname + vidtype;

  printf("loading from video %s\n", srcfile.c_str());
  printf("corner [%d, %d]\n", corner.x, corner.y);
  printf("wall point [%d, %d]\n", wallpt.x, wallpt.y);
  printf("wall angle %d\n", angle);

  return srcfile;
}


void writeCSV(std::string fname, cv::Mat mat)
{
  std::ofstream myfile;
  myfile.open(fname.c_str());
  myfile << cv::format(mat, cv::Formatter::FMT_CSV) << std::endl;
  myfile.close();
}

void writePNG(std::string fname, cv::Mat mat)
{
  std::vector<int> compression_params;
  compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(9);
  cv::imwrite(fname, mat, compression_params);
}
