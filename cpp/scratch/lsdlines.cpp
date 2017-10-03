#include "scratch_utils.cpp"


int main()
{
  int test_vid = 2;
  bool color = 0;

  cv::Mat img1, img2;
  cv::Point corner;
  int wall_angle;

  loadFrameAndInfo(test_vid, color, img1, corner, wall_angle);

  double angle = 5;
  rotateIm(img1, img2, angle);
  printf("Images loaded and ready\n");

  cv::Ptr<cv::LineSegmentDetector> lsd =
    cv::createLineSegmentDetector(cv::LSD_REFINE_STD);
  std::vector<cv::Vec4f> lineseg1, lineseg2;

  int r = 50;
  TimeVar start_t = timeNow();

  cv::Mat crop1 = img1.rowRange(corner.y-r, corner.y+r)
    .colRange(corner.x-r, corner.x+r);
  cv::Mat crop2 = img2.rowRange(corner.y-r, corner.y+r)
    .colRange(corner.x-r, corner.x+r);

  lsd->detect(crop1, lineseg1);
  lsd->detect(crop2, lineseg2);

  cv::Mat edge1 = cv::Mat::zeros(crop1.rows, crop1.cols, CV_8UC1);
  cv::Mat edge2 = cv::Mat::zeros(crop2.rows, crop2.cols, CV_8UC1);

  lsd->drawSegments(edge1, lineseg1);
  cv::cvtColor(edge1, edge1, CV_BGR2GRAY);
  lsd->drawSegments(edge2, lineseg2);
  cv::cvtColor(edge2, edge2, CV_BGR2GRAY);

  TimeVar end_t = timeNow();

  std::cout << "time elapsed: ";
  std::cout << duration(end_t - start_t) << "ms" << std::endl;

  cv::Mat edgeout;
  cv::hconcat(edge1, edge2, edgeout);
  cv::namedWindow("lineseg", cv::WINDOW_NORMAL);
  cv::imshow("lineseg", edgeout);

  cv::waitKey(0);


  /* detect hough lines */
  int thresh = 20;
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
  drawHoughLines(lines1, crop1, 1);

  printf("img 2 %lu lines detected\n", lines2.size());
  drawHoughLines(lines2, crop2, 1);

  cv::Mat out1, out2;
  vconcat(edge1, crop1, out1);
  vconcat(edge2, crop2, out2);

  cv::namedWindow("edges1", cv::WINDOW_NORMAL);
  cv::imshow("edges1", out1);

  cv::namedWindow("edges2", cv::WINDOW_NORMAL);
  cv::imshow("edges2", out2);

  cv::waitKey(0);
  cv::destroyAllWindows();

  return 0;
}
