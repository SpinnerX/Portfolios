#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <ctime>
#include <thread>
#include "BackgroundParser.h"
#include "SkinDetection.h"
#include "FaceDetector.h"
#include "FingerPoints.h"
using namespace cv;

class Camera{
public:
    Camera();
    Camera(Mat& image,int type=0);
    void calibrate(Mat&);
    // void calibrate(Mat& image); // calibrating camera

    void operator()(Mat& image);

private:
    int cameraType;
    BackgroundParser parser;
    SkinDetection skinDet;
    FaceDetector faceDet;
    FingerPoints fingerPoints;
};