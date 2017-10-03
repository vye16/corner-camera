#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"

#include "scratch_utils.cpp"


int main()
{
  int test_vid = 3;
  bool color = 0;

  cv::Mat img1, img2;
  cv::Point corner;
  int wall_angle;

  loadFrameAndInfo(test_vid, color, img1, corner, wall_angle);

  double angle = 5;
  rotateIm(img1, img2, angle);
  printf("Images loaded and ready\n");


  /* detect features */
  cv::Ptr<cv::Feature2D> orb, brisk;

  int nfeatures = 50;
  orb = cv::ORB::create(nfeatures);

  int thresh = 30;
  int noctaves = 4;
  double pattern_scale = 1;
  brisk = cv::BRISK::create(thresh, noctaves, pattern_scale);

  int r = 40;
  cv::Mat mask = cv::Mat::zeros(img1.rows, img1.cols, CV_8UC1);
  mask.rowRange(corner.y-r, corner.y+r)
    .colRange(corner.x-r, corner.x+r) = 1;

  std::vector<cv::KeyPoint> pts1, pts2;
  cv::Mat desc1, desc2;
  cv::Mat imkey1, imkey2;

  int detect_type = 0;
  TimeVar start_t, end_t;
  switch (detect_type) {
    case 0: /* brisk */
      printf("using brisk detection\n");
      start_t = timeNow();
      brisk->detectAndCompute(img1, mask, pts1, desc1);
      brisk->detectAndCompute(img2, mask, pts2, desc2);
      end_t = timeNow();
      break;
    case 1: /* orb */
      printf("using orb detection\n");
      start_t = timeNow();
      orb->detectAndCompute(img1, mask, pts1, desc1);
      orb->detectAndCompute(img2, mask, pts2, desc2);
      end_t = timeNow();
      break;
  }
  
  std::cout << "time elapsed: ";
  std::cout << duration(end_t - start_t);
  std::cout << "ms" << std::endl;

//  cv::FlannBasedMatcher matcher(
//      new cv::flann::LshIndexParams(12, 10, 2));
  cv::BFMatcher matcher(cv::NORM_HAMMING);
  std::vector<cv::DMatch> matches;
  matcher.match(desc1, desc2, matches);
  
  cv::Mat immatch;
  cv::drawMatches(img1, pts1, img2, pts2, matches, immatch);
  cv::namedWindow("matches", cv::WINDOW_NORMAL);
  cv::imshow("matches", immatch);
  cv::waitKey(0);

  cv::destroyAllWindows();

  return 0;
}
