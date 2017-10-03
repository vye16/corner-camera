#include <algorithm>
#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

#include "CalibrationWindow.h"
#include "scratch_utils.cpp"


bool response_comp(const cv::KeyPoint& p1, const cv::KeyPoint& p2)
{ return p1.response > p2.response; }

int main()
{

  std::string datadir = "/Users/vickieye/Dropbox (MIT)/shadowImaging/wheelchair_test_videos/";
  std::string srcfile = datadir + "stata_level_4_v2.avi";

  cv::VideoCapture cap(srcfile);

  if (!cap.isOpened()) {
    printf("Cannot read video from %s\n", srcfile.c_str());
    return -1;
  }

  cv::Mat frame1, frame2;
  int fps = cap.get(CV_CAP_PROP_FPS);

  // skip to 3 seconds, when we're moving
  bool ok = 1;
  for (size_t i = 0; i < fps * 3; i++) { ok = cap.grab(); }
  if (!ok) {
    printf("Cannot get to desired place in video\n");
    return -1;
  }

  if (!cap.read(frame1)) {
    printf("Cannot read first frame\n");
    return -1;
  }

  int nlevs = 1;
  int down_f = 1 << nlevs;
  for (int i = 0; i < nlevs; i++) {
    cv::pyrDown(frame1, frame1);
  }

//  CalibrationWindow win("test", frame1);
//  win.show();
//  cv::Point corner = win.corner();
//  cv::Point wallpt = win.wallPoint();

  std::cout << down_f << std::endl;
  cv::Point corner(644/down_f, 377/down_f);
  cv::Point wallpt(805/down_f, 377/down_f);

  float wall_angle = atan2(wallpt.y - corner.y, wallpt.x - corner.x);

  std::cout << "corner: " << corner << std::endl;
  std::cout << "wallpt: " << wallpt << std::endl;
  std::cout << "wall_angle: " << wall_angle << std::endl;

  // read in the next frame
  cap.read(frame2);
  for (int i = 0; i < nlevs; i++) {
    cv::pyrDown(frame2, frame2);
  }


  /* ===== FINDING KEY POINTS ===== */

  int r = 80/down_f;
  cv::Mat crop1, crop2, crop;
  crop1 = frame1.rowRange(corner.y-r, corner.y+r)
    .colRange(corner.x-r, corner.x+r);
  crop2 = frame2.rowRange(corner.y-r, corner.y+r)
    .colRange(corner.x-r, corner.x+r);

  cv::cvtColor(crop1, crop1, CV_BGR2GRAY);
  cv::cvtColor(crop2, crop2, CV_BGR2GRAY);

  cv::Ptr<cv::Feature2D> orb, brisk, surf;
  std::vector<cv::KeyPoint> pts1, pts2;
  TimeVar start_t, end_t;

  bool do_orb = 1;
  bool do_brisk = 0;
  bool do_surf = 1;

  int nfeatures = 500;
  orb = cv::ORB::create(nfeatures);
  if (do_orb) {
    start_t = timeNow();

    orb->detect(crop1, pts1);
    orb->detect(crop2, pts2);

    end_t = timeNow();
    std::cout << "orb time: ";
    std::cout << duration(end_t - start_t);
    std::cout << "ms" << std::endl;
    printf("frame 1 %lu points detected so far\n", pts1.size());
    printf("frame 2 %lu points detected so far\n", pts2.size());
  }

  int thresh = 20;
  int noctaves = 4;
  double pattern_scale = 1;
  brisk = cv::BRISK::create(thresh, noctaves, pattern_scale);
  if (do_brisk) {
    start_t = timeNow();
    brisk->detect(crop1, pts1);
    brisk->detect(crop2, pts2);

    end_t = timeNow();
    std::cout << "brisk time: ";
    std::cout << duration(end_t - start_t);
    std::cout << "ms" << std::endl;
    printf("frame 1 %lu points detected so far\n", pts1.size());
    printf("frame 2 %lu points detected so far\n", pts2.size());
  }

  double hessian_thresh = 500;
  surf = cv::xfeatures2d::SURF::create(hessian_thresh);
  if (do_surf) {
    start_t = timeNow();
    surf->detect(crop1, pts1);
    surf->detect(crop2, pts2);

    end_t = timeNow();
    std::cout << "surf time: ";
    std::cout << duration(end_t - start_t);
    std::cout << "ms" << std::endl;
    printf("frame 1 %lu points detected so far\n", pts1.size());
    printf("frame 2 %lu points detected so far\n", pts2.size());
  }

  std::sort(pts1.begin(), pts1.end(), response_comp);
  std::sort(pts2.begin(), pts2.end(), response_comp);

  cv::Mat imkey1, imkey2, imkey;
  cv::drawKeypoints(crop1, pts1, imkey1);
  cv::drawKeypoints(crop2, pts2, imkey2);
  cv::hconcat(imkey1, imkey2, imkey);

  cv::namedWindow("features", cv::WINDOW_NORMAL);
  cv::imshow("features", imkey);
  cv::waitKey(0);

  /* ===== FINDING THE WALL ===== */

  cv::Ptr<cv::LineSegmentDetector> lsd =
    cv::createLineSegmentDetector(cv::LSD_REFINE_STD);
  std::vector<cv::Vec4f> lineseg1, lineseg2;

  // get line segments
  start_t = timeNow();

  lsd->detect(crop1, lineseg1);
  lsd->detect(crop2, lineseg2);

  cv::Mat edge1 = cv::Mat::zeros(crop1.rows, crop1.cols, CV_8UC1);
  cv::Mat edge2 = cv::Mat::zeros(crop2.rows, crop2.cols, CV_8UC1);

  lsd->drawSegments(edge1, lineseg1);
  cv::cvtColor(edge1, edge1, CV_BGR2GRAY);
  lsd->drawSegments(edge2, lineseg2);
  cv::cvtColor(edge2, edge2, CV_BGR2GRAY);

  end_t = timeNow();

  std::cout << "lsd lines time: ";
  std::cout << duration(end_t - start_t) << "ms" << std::endl;

  cv::Mat edgeout;
  cv::hconcat(edge1, edge2, edgeout);
  cv::namedWindow("lineseg", cv::WINDOW_NORMAL);
  cv::imshow("lineseg", edgeout);

  cv::waitKey(0);

  // hough lines
  thresh = 20;
  double rho_res = 1;
  double theta_res = CV_PI/180;
  double wall_norm = (double)(wall_angle - 90)*CV_PI/180;
  double theta_win = 5 * theta_res;
  printf("wall_norm %.2f\n", wall_norm);
  printf("checking thetas from %.2f to %.2f\n",
      wall_norm - theta_win, wall_norm + theta_win);

  std::vector<cv::Vec2f> lines1, lines2;
  start_t = timeNow();
  cv::HoughLines(edge1, lines1, rho_res, theta_res, thresh,
      0, 0, wall_norm - theta_win, wall_norm + theta_win);
  cv::HoughLines(edge2, lines2, rho_res, theta_res, thresh,
      0, 0, wall_norm - theta_win, wall_norm + theta_win);
  end_t = timeNow();
  std::cout << "hough lines time elapsed: ";
  std::cout << duration(end_t - start_t) << "ms" << std::endl;

  printf("img 1 %lu lines detected\n", lines1.size());
  drawHoughLines(lines1, imkey1, 1);

  printf("img 2 %lu lines detected\n", lines2.size());
  drawHoughLines(lines2, imkey2, 1);

  cv::hconcat(imkey1, imkey2, imkey);

  cv::namedWindow("lines", cv::WINDOW_NORMAL);
  cv::imshow("lines", imkey);

  cv::waitKey(0);

  // crude estimate for new corner
  cv::Point newcorner;
  newcorner.x = pts2.pt.x + (corner.x - pts1.pt.x);
  newcorner.y = pts2.pt.y + (corner.y - pts1.pt.y);

  cv::destroyAllWindows();

  return 0;
}
