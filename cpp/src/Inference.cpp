#include "Inference.h"


Inference::Inference(int nangles, int ncircles,
    float rstep, double lambda, double beta, double alpha)
{
  downlevs_ = 2;
  nangles_ = nangles;
  ncircles_ = ncircles;
  rstep_ = rstep;
  lambda_ = lambda;
  beta_ = beta;
  alpha_ = alpha;

  corner_x_ = 0;
  corner_y_ = 0;
  theta_start_ = 0;
  theta_end_ = 0;
}


Inference::Inference(cv::Point corner,
    cv::Point wallpt, cv::Point endpt,
    int nangles, int ncircles, float rstep,
    double lambda, double beta, double alpha)
{
  setObsRegion(corner, wallpt, endpt);

  nangles_ = nangles;
  ncircles_ = ncircles;
  rstep_ = rstep;
  lambda_ = lambda;
  beta_ = beta;
  alpha_ = alpha;
  downlevs_ = 2;
}


void Inference::findObsRegion()
{
  // assume that corner and theta limits aren't the default values
  // they've already been set
  if (theta_start_ != theta_end_ &&
      corner_x_ != 0 && corner_y_ != 0) {
    printf("Using input values: corner (%f, %f), thetas %f to %f\n",
        corner_x_, corner_y_, theta_start_, theta_end_);
    return;
  }
  if (frame_.rows <= 0 || frame_.cols <= 0) {
    throw std::runtime_error("No frame has been read yet");
  }
  printf("Getting observation region from user\n");
  CalibrationWindow win("calibration", frame_);
  win.show(); // get user input

  setObsRegion(win.corner(), win.wallPoint(), win.endPoint());
}


void Inference::setObsRegion(cv::Point corner,
    cv::Point wallpt, cv::Point endpt)
{
  corner_x_ = (float) corner.x;
  corner_y_ = (float) corner.y;

  float wall_x = (float) wallpt.x;
  float wall_y = (float) wallpt.y;

  float end_x = (float) endpt.x;
  float end_y = (float) endpt.y;

  /* angle from 0 clockwise to wall */
  float wall_angle = atan2(wall_y - corner_y_, wall_x - corner_x_);
  float end_angle = atan2(end_y - corner_y_, end_x - corner_x_);
  wall_angle = fmod(wall_angle + 2*CV_PI, 2*CV_PI);
  end_angle = fmod(end_angle + 2*CV_PI, 2*CV_PI);

  float diff_angle = end_angle - wall_angle;
  float angle_dir = diff_angle / abs(diff_angle);
  printf("%f, %f, diff angle %f, angle dir %f\n",
      wall_angle, end_angle, diff_angle, angle_dir);

  theta_start_ = wall_angle;
  if (abs(diff_angle) < CV_PI) {
    printf("1\n");
    theta_end_ = wall_angle + diff_angle;
  } else {
    printf("2\n");
    diff_angle = 2*CV_PI - abs(diff_angle);
    theta_end_ = wall_angle - angle_dir * diff_angle;
  }
}


void Inference::setNumSamples(int nangles, int ncircles)
{
  nangles_ = nangles;
  ncircles_ = ncircles;
}


void Inference::setup()
{
  setObsXYLocs();
  setAmat();
  setRegularizer();

  /* compute gain */
  cv::Mat tmp = amat_.t()*(amat_/lambda_) + beta_*reg_;
  gain_ = tmp.inv()*(amat_.t()/lambda_); 
}


void Inference::processFrame(cv::Mat& cur_disp)
{ /* process the frame currently in dframe_ */
  cv::Mat samples;
  cv::remap(dframe_ - backim_, samples,
      xq_, yq_, CV_INTER_LINEAR);

  cv::Mat samples_bgr[3];
  cv::split(samples, samples_bgr);

  /* inference for each channel */
  cv::Mat cur_out;
  cv::Mat dispchans[3];
  for (int i = 0; i < 3; i++) {
    dispchans[i] = cv::Mat(1, nangles_, CV_64FC1);

    cur_out = (gain_ * (samples_bgr[i] + meanpx_[i])).t();
    cur_out = cur_out.colRange(1, amat_.cols);
    cur_out.copyTo(dispchans[i]);
  }
  cv::merge(dispchans, 3, cur_disp);
}


void Inference::updateMeanImage(int total)
{
  if (total == 1) {
    dframe_.copyTo(backim_);
  } else {
//    backim_ = backim_ * (total-1) / total + dframe_ / total;
    backim_ = alpha_ * backim_ + (1 - alpha_) * dframe_;
  }
  meanpx_ = cv::mean(backim_);
}


void Inference::preprocessFrame()
{
  // cast to double, crop to region of interest
  // then blur and downsample the frame
  frame_.convertTo(dframe_, CV_64FC3);
  dframe_ = dframe_.rowRange(ymin_, ymax_)
    .colRange(xmin_, xmax_);
  for (int i = 0; i < downlevs_; i++) {
    cv::pyrDown(dframe_, dframe_);
  }
}


void Inference::setObsXYLocs()
{
  setPieSliceXYLocs(); // will incorporate other sampling as well

  // set the parameters for preprocessing
  double xmin, xmax, ymin, ymax;
  cv::minMaxLoc(xq_, &xmin, &xmax);
  cv::minMaxLoc(yq_, &ymin, &ymax);

  float pad = 5 * (1 << (downlevs_ + 1));
  xmin_ = fmax(0, round(xmin) - pad);
  ymin_ = fmax(0, round(ymin) - pad);
  xmax_ = fmin(ncols_, round(xmax) + pad);
  ymax_ = fmin(nrows_, round(ymax) + pad);

  // transform query points for the processed frame's coordinates
  xq_ = (xq_ - xmin_)/(1<<downlevs_);
  yq_ = (yq_ - ymin_)/(1<<downlevs_);
}


void Inference::setPieSliceXYLocs()
{
  cv::Mat xq = cv::Mat::zeros(ncircles_, nangles_, CV_32FC1);
  cv::Mat yq = cv::Mat::zeros(ncircles_, nangles_, CV_32FC1);
  cv::Mat tq = cv::Mat::zeros(ncircles_, nangles_, CV_32FC1);

  float *xrow, *yrow, *trow;
  float rcur = 0;
  float acur = 0;
  float astep = (theta_end_ - theta_start_) / (nangles_ - 1);

  for (int ci = 0; ci < ncircles_; ci++) {
    rcur += rstep_;
    acur = theta_start_;
    xrow = xq.ptr<float>(ci);
    yrow = yq.ptr<float>(ci);
    trow = tq.ptr<float>(ci);
    for (int ai = 0; ai < nangles_; ai++) {
      xrow[ai] = corner_x_ + rcur * cos(acur);
      yrow[ai] = corner_y_ + rcur * sin(acur);
      trow[ai] = acur;
      acur += astep;
    }
  }

  xq_ = xq.reshape(0, ncircles_ * nangles_);
  yq_ = yq.reshape(0, ncircles_ * nangles_);
  tq_ = tq.reshape(0, ncircles_ * nangles_);
}


void Inference::plotObsXYLocs(std::string winname)
{
  cv::namedWindow(winname, CV_WINDOW_NORMAL);
  cv::Mat samples;
  dframe_.convertTo(samples, CV_8UC3);
  cv::imshow(winname, samples);

  cv::circle(samples, cv::Point((corner_x_ - xmin_)/(1<<downlevs_),
        (corner_y_ - ymin_)/(1<<downlevs_)), 1, cv::Scalar(255,0,0));
  for (int i = 0; i < ncircles_*nangles_; i++) {
    cv::circle(samples, cv::Point(round(xq_.at<float>(i)),
            round(yq_.at<float>(i))), 1, cv::Scalar(255,0,0));
  }
  cv::imshow(winname, samples);
}


void Inference::setAmat()
{
  int nobs = tq_.rows * tq_.cols;
  float tdelta = (theta_end_ - theta_start_) / (nangles_ - 1);
  float tdir = tdelta / std::abs(tdelta);

  // make a row vec of angles we're reconstructing at
  cv::Mat thetas = cv::Mat::zeros(1, nangles_, CV_32FC1);
  float *trow = thetas.ptr<float>(0);
  float tcur = theta_start_;
  for (int i = 0; i < nangles_; i++) {
    trow[i] = tcur; tcur += tdelta;
  }

  int idx;
  double *arow;
  double diff;
  float angle;
  cv::Mat mask;
  amat_ = cv::Mat::zeros(nobs, nangles_ + 1, CV_64FC1);
  for (int i = 0; i < nobs; i++) {
    arow = amat_.ptr<double>(i);
    angle = tq_.at<float>(i,0);
    mask = ((angle - thetas) * tdir >= 0)/255;
    // find the idx of the last theta we can see at this angle
    idx = (int) cv::sum(mask)[0] - 1;
    if (idx < 0) { // doesn't see any of the scene
      arow[0] = 1; // still sees the constant light
    } else { // angle indexing starts at 1
      diff = (double) (angle - trow[idx]) / tdelta;
      for (int j = 0; j <= idx; j++) {
        arow[j] = 1;
      }
      arow[idx+1] = 0.5 * (2 - diff) * diff + 0.5;
      if (idx < nangles_) {
        arow[idx+2] = 0.5 * diff * diff;
      }
    }
  }
}


void Inference::setRegularizer()
{
  int xdim = amat_.cols;
  cv::Mat bmat = cv::Mat::zeros(xdim-1, xdim, CV_64FC1);

  double *row;
  for (int i = 1; i < bmat.rows; i++) {
    row = bmat.ptr<double>(i);
    row[i] = 1;
    row[i+1] = -1;
  }

  cv::Mat cmat = cv::Mat::eye(xdim, xdim, CV_64FC1);
  cmat.at<double>(0, 0) = 0;

  reg_ = bmat.t() * bmat + cmat;
}
