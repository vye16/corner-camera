#include "PointGreyInference.h"


PointGreyInference::PointGreyInference( int nangles, int ncirlces,
    float rstep, double lambda, double beta, double alpha)
  : Inference(nangles, ncirlces, rstep, lambda, beta, alpha)
{
}


PointGreyInference::PointGreyInference(cv::Point corner,
    cv::Point wallpt, cv::Point endpt,
    int nangles, int ncirlces, float rstep,
    double lambda, double beta, double alpha)
  : Inference(corner, wallpt, endpt,
      nangles, ncirlces, rstep, lambda, beta, alpha)
{
}


void PointGreyInference::getNextFrame(FlyCapture2::Camera *cam)
{
  FlyCapture2::Image raw, rgb;
  FlyCapture2::Error error = cam->RetrieveBuffer(&raw);
  if (error != FlyCapture2::PGRERROR_OK) {
    throw std::runtime_error("capture error");
  }
  raw.Convert(FlyCapture2::PIXEL_FORMAT_BGR, &rgb);
  unsigned int rowbytes = (double)rgb.GetReceivedDataSize()
    /(double) rgb.GetRows();
  cv::Mat frame;
  frame = cv::Mat(rgb.GetRows(), rgb.GetCols(),
      CV_8UC3, rgb.GetData(), rowbytes);
  frame.copyTo(frame_);
}


void PointGreyInference::processStream(bool disp_rollup)
{ // if disp_rollup > 0, roll up, otherwise roll down
  cv::destroyAllWindows();

  FlyCapture2::Error error;
  FlyCapture2::Camera cam;
  FlyCapture2::CameraInfo camInfo;

  // connect to camera
  error = cam.Connect(0);
  if (error != FlyCapture2::PGRERROR_OK) {
    throw std::runtime_error("Failed to connect to camera");
  }

  error = cam.GetCameraInfo(&camInfo);
  if (error != FlyCapture2::PGRERROR_OK) {
    throw std::runtime_error("Failed to get camera info from camera");
  }
  printf("%s %s %d\n", camInfo.vendorName,
      camInfo.modelName, camInfo.serialNumber);

  error = cam.StartCapture();
  if (error == FlyCapture2::PGRERROR_ISOCH_BANDWIDTH_EXCEEDED) {
    throw std::runtime_error("Bandwidth exceeded");
  } else if (error != FlyCapture2::PGRERROR_OK) {
    throw std::runtime_error("Failed to start image capture");
  }
  

  autoAdjustProperty(&cam, FlyCapture2::GAIN);
  autoAdjustProperty(&cam, FlyCapture2::SHUTTER);
  autoAdjustProperty(&cam, FlyCapture2::TEMPERATURE);
  setFPS(&cam, 30);


  // get frame info, setup, and initialize running mean
  getNextFrame(&cam);
  nrows_ = frame_.rows;
  ncols_ = frame_.cols;
  printf("%d x %d framesize\n", nrows_, ncols_);

  findObsRegion(); // set observation region if haven't already

  setup(); // compute gain, and sample and crop locations

  // initialize the background image
  preprocessFrame();
  updateMeanImage(1);

  plotObsXYLocs("sample locations");

  display = RollingDisplay("output", disp_rollup, nangles_);
  cv::Mat cur_disp;

  cv::namedWindow("camera", CV_WINDOW_NORMAL);
  cv::namedWindow("background image", CV_WINDOW_NORMAL);

  /* capture loop */
  printf("Starting inference, press esc to break\n");
  time_t start_time = time(0);

  int total = 0;
  while (1) {
    getNextFrame(&cam);
    preprocessFrame();
    cur_disp = display.nextrow();
    processFrame(cur_disp);

    cv::imshow("camera", dframe_/255);
    cv::imshow("background image", backim_/255);

    display.update();
    if (cv::waitKey(1) == 27) {
      printf("pressed ESC key, exiting inference\n");
      break;
    }

    updateMeanImage(total+2);
    total++;
  }

  error = cam.StopCapture();
  if (error != FlyCapture2::PGRERROR_OK) {
    printf("error stopping capture\n");
  }

  cam.Disconnect();
  cv::destroyAllWindows();
}


void PointGreyInference::disableAuto(FlyCapture2::Camera *cam,
    FlyCapture2::PropertyType propType)
{
  FlyCapture2::Property prop;
  prop.type = propType;
  FlyCapture2::Error error = cam->GetProperty(&prop);
  if (error != FlyCapture2::PGRERROR_OK) {
    error.PrintErrorTrace();
  }

  prop.autoManualMode = false;
  prop.absControl = true;
  error = cam->SetProperty(&prop);
  if (error != FlyCapture2::PGRERROR_OK) {
    error.PrintErrorTrace();
  }
}


void PointGreyInference::setProperty(FlyCapture2::Camera *cam,
    FlyCapture2::PropertyType propType, float ss)
{
  FlyCapture2::Property prop;
  prop.type = propType;
  FlyCapture2::Error error = cam->GetProperty(&prop);
  if (error != FlyCapture2::PGRERROR_OK) {
    error.PrintErrorTrace();
  }

  prop.absValue = ss;
  prop.autoManualMode = false;
  prop.absControl = true;
  error = cam->SetProperty(&prop);
  if (error != FlyCapture2::PGRERROR_OK) {
    error.PrintErrorTrace();
  }

  printf("property requested value: %f\n", ss);
  cam->GetProperty(&prop);
  printf("property received value: %f\n", prop.absValue);
}


void PointGreyInference::setExposure(FlyCapture2::Camera *cam, float ss)
{
  printf("setting exposure (shutter)...\n");
  setProperty(cam, FlyCapture2::SHUTTER, ss);
}


void PointGreyInference::setGain(FlyCapture2::Camera *cam, float ss)
{
  printf("setting gain...\n");
  setProperty(cam, FlyCapture2::GAIN, ss);
}


void PointGreyInference::setFPS(FlyCapture2::Camera *cam, float ss)
{
  printf("setting fps...\n");
  setProperty(cam, FlyCapture2::FRAME_RATE, ss);
}


void PointGreyInference::autoAdjustProperty(FlyCapture2::Camera *cam,
    FlyCapture2::PropertyType propType)
{
  FlyCapture2::Property prop;
  prop.type = propType;
  FlyCapture2::Error error = cam->GetProperty(&prop);
  if (error != FlyCapture2::PGRERROR_OK) {
    error.PrintErrorTrace();
  }

  prop.autoManualMode = false;
  prop.absControl = true;
  prop.onePush = true;
  error = cam->SetProperty(&prop);
  if (error != FlyCapture2::PGRERROR_OK) {
    error.PrintErrorTrace();
  }
}


