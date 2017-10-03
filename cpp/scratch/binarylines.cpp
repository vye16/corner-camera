#include "opencv2/line_descriptor.hpp"

#include "scratch_utils.cpp"

typedef cv::line_descriptor::KeyLine KeyLine;
typedef cv::line_descriptor::BinaryDescriptor BinaryDescriptor;

void drawBDLines(cv::Mat& img, std::vector<KeyLine> lines,
    int octave=0)
{
  KeyLine l;
  cv::Point p1, p2;
  for (int i = 0; i < lines.size(); i++) {
    l = lines[i];
    if (l.octave == octave) {
      p1 = cv::Point(l.startPointX, l.startPointY);
      p2 = cv::Point(l.endPointX, l.endPointY);
      cv::line(img, p1, p2, cv::Scalar(255, 255, 255), 5);
    }
  }
}


int main()
{
  int test_vid = 1;
  bool color = 0;

  cv::Mat img1, img2;
  cv::Point corner;
  int wall_angle;

  loadFrameAndInfo(test_vid, color, img1, corner, wall_angle);

  double angle = 5;
  rotateIm(img1, img2, angle);
  printf("Images loaded and ready\n");

  int r = 50;
  cv::Mat crop1 = img1.rowRange(corner.y-r, corner.y+r)
    .colRange(corner.x-r, corner.x+r);
  cv::Mat crop2 = img2.rowRange(corner.y-r, corner.y+r)
    .colRange(corner.x-r, corner.x+r);

  cv::Ptr<BinaryDescriptor> bd =
    BinaryDescriptor::createBinaryDescriptor();
  std::vector<KeyLine> lines1, lines2;

  TimeVar start_t = timeNow();
  bd->detect(crop1, lines1);
  bd->detect(crop2, lines2);
  TimeVar end_t = timeNow();

  std::cout << "time elapsed: ";
  std::cout << duration(end_t - start_t) << "ms" << std::endl;

  printf("img 1 %lu lines detected\n", lines1.size());
  printf("img 2 %lu lines detected\n", lines2.size());

  drawBDLines(crop1, lines1, 1);
  drawBDLines(crop2, lines2, 1);

  cv::Mat cropout;
  cv::hconcat(crop1, crop2, cropout);
  cv::namedWindow("lines", cv::WINDOW_NORMAL);
  cv::imshow("lines", cropout);

  cv::waitKey(0);
  cv::destroyAllWindows();

  return 0;
}
