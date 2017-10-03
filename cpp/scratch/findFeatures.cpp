#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

#include "scratch_utils.cpp"


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

  bool do_orb = 1;
  bool do_brisk = 1;
  bool do_sift = 0;
  bool do_surf = 0;

  int r = 50;
  cv::Mat mask = cv::Mat::zeros(img1.rows, img1.cols, CV_8UC1);
  mask.rowRange(corner.y-r, corner.y+r)
    .colRange(corner.x-r, corner.x+r) = 1;

  std::vector<cv::Ptr<cv::Feature2D> > ds;
  std::vector<std::string> names;

  int nfeatures = 20;
  if (do_orb) {
    ds.push_back(cv::ORB::create(nfeatures));
    names.push_back("orb");
  }

  int thresh = 20;
  int noctaves = 4;
  double pattern_scale = 1;
  if (do_brisk) {
    ds.push_back(cv::BRISK::create(thresh, noctaves, pattern_scale));
    names.push_back("brisk");
  }

  int noctave_layers = 3;
  double contrast_thresh = 0.01;
  double edge_thresh = 8;
  double sigma = 1.6;
  if (do_sift) {
    ds.push_back(cv::xfeatures2d::SIFT::create(nfeatures,
          noctave_layers, contrast_thresh, edge_thresh, sigma));
    names.push_back("sift");
  }

  double hessian_thresh = 500;
  if (do_surf) {
    ds.push_back(cv::xfeatures2d::SURF::create(hessian_thresh));
    names.push_back("surf");
  }

  std::vector<cv::KeyPoint> pts1, pts2;
  cv::Mat imkey1, imkey2, imkey;

  TimeVar start_t, end_t;

  for (int i = 0; i < ds.size(); i++) {
    start_t = timeNow();
    ds[i]->detect(img1, pts1, mask);
    ds[i]->detect(img2, pts2, mask);
    end_t = timeNow();

    std::cout << names[i] << " time: ";
    std::cout << duration(end_t - start_t);
    std::cout << "ms" << std::endl;

    cv::drawKeypoints(img1, pts1, imkey1);
    cv::drawKeypoints(img2, pts2, imkey2);

    cv::vconcat(imkey1, imkey2, imkey);
    cv::namedWindow(names[i], cv::WINDOW_NORMAL);
    cv::imshow(names[i], imkey);
  }

  cv::waitKey(0);

  cv::destroyAllWindows();

  return 0;
}
