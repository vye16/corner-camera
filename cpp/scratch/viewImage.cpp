// Example showing how to read and write imgs
#include <vector>
#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
  std::string dirname = "/Users/vickieye/Projects/freeman/";
  dirname += "corner-camera-demo/cpp/scratch/";
  std::string fname = dirname + "frame1.png";

  Mat img;
  img = imread(fname, 1 );
  if ( !img.data ) {
    printf("No img data \n");
    return -1;
  }

  namedWindow("img", WINDOW_AUTOSIZE);
  imshow("img", img);

  waitKey(0);
  destroyWindow("img");

  if (1) {
    cv::Mat resized;
    cv::resize(img, resized, cv::Size(img.cols/2, img.rows/2));

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    fname = dirname + "frame_small.png";
    imwrite(fname, resized, compression_params);
  }
  return 0;
}
