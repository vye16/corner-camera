#include "FlyCapture2.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <iostream>
#include <ctime>

using namespace FlyCapture2;
using namespace std;
using namespace cv;

string opencv_window_name = "pointgrey_opencv";

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        cout << "pure filename (please don't include fileformat): " << argv[1] << endl;
    } else {
        cout << "Not enough arguments" << endl;
        return false;
    }

    FlyCapture2::Error error;
    Camera camera;
    CameraInfo camInfo;

    // Connect the camera
    error = camera.Connect( 0 );
    if ( error != PGRERROR_OK )
    {
        cout << "Failed to connect to camera" << endl;     
        return false;
    }

    // Get the camera info and print it out
    error = camera.GetCameraInfo( &camInfo );
    if ( error != PGRERROR_OK )
    {
        cout << "Failed to get camera info from camera" << endl;     
        return false;
    }
    cout << camInfo.vendorName << " "
              << camInfo.modelName << " " 
              << camInfo.serialNumber << endl;
	
    error = camera.StartCapture();
    if ( error == PGRERROR_ISOCH_BANDWIDTH_EXCEEDED )
    {
        cout << "Bandwidth exceeded" << endl;     
        return false;
    }
    else if ( error != PGRERROR_OK )
    {
        cout << "Failed to start image capture" << endl;     
        return false;
    }

    cout << "Starting video writing ..." << endl;

    // params for video writing
    time_t t = time(0);
    struct tm * now = localtime( & t );
    string date = to_string((now->tm_year + 1900)) + "-" + to_string((now->tm_mon + 1)) + "-" + to_string(now->tm_mday);
    const string outputname = date + "_" + string(argv[1]) + string(".avi");
    cout << outputname << endl;
    Size S = Size((int) 1928,
                  (int) 1448);
    int fourcc = VideoWriter::fourcc('X','V','I','D');

    // Open the output
    VideoWriter outputVideo;
    outputVideo.open(outputname, fourcc, 20, S, true);
    cout << "fourcc " << fourcc << endl;

    if (!outputVideo.isOpened())
    {
        cout  << "Could not open the output video for write: " << outputname << endl;
        return -1;
    }

    // capture loop
    char key = 0;
    while(key != 'q')
    {
        // Get the image
        Image rawImage;
        FlyCapture2::Error error = camera.RetrieveBuffer( &rawImage );
        if ( error != PGRERROR_OK )
        {
                cout << "capture error" << endl;
                continue;
        }

        // convert to rgb
        Image rgbImage;
        rawImage.Convert( FlyCapture2::PIXEL_FORMAT_BGR, &rgbImage );

        // convert to OpenCV Mat
        unsigned int rowBytes = (double)rgbImage.GetReceivedDataSize()/(double)rgbImage.GetRows();       
        Mat image = Mat(rgbImage.GetRows(), rgbImage.GetCols(), CV_8UC3, rgbImage.GetData(),rowBytes);
        cout << rgbImage.GetRows() << " " << rgbImage.GetCols() << endl;

        imshow(opencv_window_name, image);
        key = waitKey(30);

        outputVideo.write(image);
    }

    error = camera.StopCapture();
    if ( error != PGRERROR_OK )
    {
        // This may fail when the camera was removed, so don't show 
        // an error message
    }  

    camera.Disconnect();
    destroyWindow(opencv_window_name);

    return 0;
}
