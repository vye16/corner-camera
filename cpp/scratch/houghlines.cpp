#include <cmath>

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

  int r = 50;
  cv::Mat crop1 = img1.rowRange(corner.y-r, corner.y+r)
    .colRange(corner.x-r, corner.x+r);
  cv::Mat crop2 = img2.rowRange(corner.y-r, corner.y+r)
    .colRange(corner.x-r, corner.x+r);

  int lowthresh = 60;
  int ratio = 4;
  int kernel_size = 3;
  cv::Mat edge1, edge2;

  TimeVar start_t, end_t;

  start_t = timeNow();
  cv::Canny(crop1, edge1, lowthresh, ratio*lowthresh, kernel_size);
  cv::Canny(crop2, edge2, lowthresh, ratio*lowthresh, kernel_size);
  end_t = timeNow();

  std::cout << "canny time elapsed: ";
  std::cout << duration(end_t - start_t) << "ms" << std::endl;

  int thresh = 20;
  double rho_res = 1;
  double theta_res = CV_PI/180;
  double wall_norm = (double)(wall_angle - 90)*CV_PI/180;
  double theta_win = 5 * theta_res;
  printf("wall_norm %.2f\n", wall_norm);
  printf("checking thetas from %.2f to %.2f\n",
      wall_norm - theta_win, wall_norm + theta_win);

  std::vector<cv::Vec2f> lines1, lines2;
  std::vector<cv::Vec4i> linesp1, linesp2;
  if (1) {
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
  } else {
    start_t = timeNow();
    cv::HoughLinesP(edge1, linesp1, rho_res, theta_res, thresh);
    cv::HoughLinesP(edge2, linesp2, rho_res, theta_res, thresh);
    end_t = timeNow();
    std::cout << "probabilistic hough lines time elapsed: ";
    std::cout << duration(end_t - start_t) << "ms" << std::endl;

    printf("img 1 %lu lines detected\n", linesp1.size());
    drawHoughLinesP(linesp1, crop1);

    printf("img 2 %lu lines detected\n", linesp2.size());
    drawHoughLinesP(linesp2, crop2);
  }

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

