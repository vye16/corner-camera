#include "CalibrationWindow.h"


CalibrationWindow::CalibrationWindow(std::string win_name,
    const cv::Mat& cal_image)
{
  name_ = win_name;
  cv::resize(cal_image, disp_image_, cv::Size(),
      0.5, 0.5, CV_INTER_AREA);
  resize_fx_ = (float) cal_image.cols / disp_image_.cols;
  resize_fy_ = (float) cal_image.rows / disp_image_.rows;

  pt_color = CV_RGB(255, 0, 0);
  pt_size = 5;
  txt_color = CV_RGB(50, 250, 50);
  txt_font = CV_FONT_HERSHEY_PLAIN;
  txt_scale = 2;
  txt_thickness = 2;
  txt_padding = 15;
  txt_height = cv::getTextSize("A", txt_font, txt_scale,
  txt_thickness, 0).height;

  /* add user directions text */
  cv::putText(disp_image_, "Please select the corner.",
      cv::Point(txt_padding, txt_padding + txt_height),
      txt_font, txt_scale, txt_color, txt_thickness);
}


void CalibrationWindow::onClick(int event, int x, int y,
        int flags, void* args)
{
  if (event == cv::EVENT_LBUTTONDOWN) {
    printf("Mouse clicked at (%d, %d)\n", x, y);
    CalibrationWindow* win = (CalibrationWindow*) args;
    win->handleNewPoint(x, y);
  }
}

void CalibrationWindow::handleNewPoint(int x, int y)
{
  if (clicked_pts_.size() >= npts_needed) {
    return;
  }
  // add point to image
  cv::Point pt = cv::Point(x, y);
  cv::circle(disp_image_, pt, pt_size, pt_color, -1);

  pt.x = pt.x * resize_fx_;
  pt.y = pt.y * resize_fy_;
  std::cout << "added " << pt << std::endl;
  clicked_pts_.push_back(pt);

  // add next instructions
  std::string instr;
  int txt_line = clicked_pts_.size() + 1;
  switch (clicked_pts_.size()) {
    case 1:
      instr = "Now, a point at the base of the wall.";
      break;
    case 2:
      instr = "Finally, an angular endpoint.";
      break;
    case 3:
      instr = "Press any key to continue.";
      break;
  }
  cv::putText(disp_image_, instr, cv::Point(txt_padding,
      txt_line * (txt_padding + txt_height)),
      txt_font, txt_scale, txt_color, txt_thickness);
  imshow(name_, disp_image_);
}


/* shows window and collects the necessary points */
void CalibrationWindow::show() 
{
  cv::namedWindow(name_, CV_WINDOW_NORMAL);
  cv::setMouseCallback(name_, CalibrationWindow::onClick, this);

  printf("Please select the corner, a point at the base of the wall, and an ending point.\n");
  printf("Press any key to exit.\n");

  cv::imshow(name_, disp_image_);
  cv::waitKey(0);
  cv::destroyWindow(name_);
}


cv::Point CalibrationWindow::corner() {
  if (clicked_pts_.size() < 1) {
    throw std::runtime_error("Corner has not been set yet.");
  }
  return clicked_pts_[0];
}


cv::Point CalibrationWindow::wallPoint() {
  if (clicked_pts_.size() < 2) {
    throw std::runtime_error("Wall point has not been set yet.");
  }
  return clicked_pts_[1];
}


cv::Point CalibrationWindow::endPoint() {
  if (clicked_pts_.size() < 3) {
    throw std::runtime_error("End point has not been set yet.");
  }
  return clicked_pts_[2];
}
