#include <iostream>
#include <string>
#include <vector>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"


#define PI 3.14159265


struct ImageWindow 
{
  int pts_needed;
  std::string name;
  cv::Mat disp_image;
  double resize_x_factor, resize_y_factor;
  cv::Scalar pt_color;
  std::vector<cv::Point> clicked_pts;
};


void clickCallback(int event, int x, int y, int flags, void *params)
{
  if (event == cv::EVENT_LBUTTONDOWN) {
    printf("Mouse clicked at (%d, %d)\n", x, y);

    ImageWindow *win = (ImageWindow*) params;
    if (win->clicked_pts.size() >= win->pts_needed) {
      return;
    } else {
      cv::Point pt(x/win->resize_x_factor, y/win->resize_y_factor);
      std::cout << pt << std::endl;
      win->clicked_pts.push_back(pt);
      cv::circle(win->disp_image, cv::Point(x, y), 3,
      win->pt_color, -1);
      cv::imshow(win->name, win->disp_image);
    }
  }
}



int main(int, char** argv)
{
  const std::string datafolder = "/Users/vickieye/Dropbox (MIT)/shadowImaging/edgeImaging/data/testvideos_Mar04/"; 
  const std::string expfolder = datafolder + "experiments/";
  const std::string resfolder = datafolder + "results/";

  const std::string name = "blue_randomwalking1";
  const std::string vidtype = ".MP4";
  const std::string srcfile = expfolder + name + vidtype;

  cv::VideoCapture cap(srcfile);

  if (!cap.isOpened()) {
    printf("Cannot read video from %s\n", srcfile.c_str());
    return -1;
  }

  double dwidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  double dheight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
  printf("size: %.0f x %.0f\n", dwidth, dheight);


  cv::Mat frame;
  bool ok = cap.read(frame);
  if (!ok) {
    printf("Cannot read in frame\n");
    return -1;
  }

  ImageWindow win;
  win.name = "Calibration";
  win.pts_needed = 2;
  win.pt_color = CV_RGB(255, 255, 0);

  int txt_font = CV_FONT_HERSHEY_PLAIN;
  double txt_scale = 1.5;
  cv::Scalar txt_color = CV_RGB(200, 255, 220);
  int txt_thick = 1;
  int txt_pad = 15;
  int txt_height = cv::getTextSize("Hello", txt_font, txt_scale, txt_thick, 0).height;

  cv::resize(frame, win.disp_image, cv::Size(floor(dwidth/2), floor(dheight/2)));
  win.resize_x_factor = win.disp_image.cols / dwidth;
  win.resize_y_factor = win.disp_image.rows / dheight;

  cv::putText(win.disp_image, "Please select the corner,",
  cv::Point(txt_pad, txt_pad + txt_height),
  txt_font, txt_scale, txt_color, txt_thick);
  cv::putText(win.disp_image, "a point at the base of the wall,",
  cv::Point(txt_pad, 2*(txt_pad + txt_height)),
  txt_font, txt_scale, txt_color, txt_thick);
  cv::putText(win.disp_image, "and a point in the observation region.",
  cv::Point(txt_pad, 3*(txt_pad + txt_height)),
  txt_font, txt_scale, txt_color, txt_thick);

  cv::namedWindow(win.name, CV_WINDOW_NORMAL);
  cv::setMouseCallback(win.name, clickCallback, &win);

  cv::imshow(win.name, win.disp_image);

  //    int key = cv::waitKey(0);
  //    std::cout << "Key " << key << " pressed." << std::endl;

  return 0;

}
