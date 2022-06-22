#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;

class BackgroundParser{
public:
    BackgroundParser();

    void calibrate(Mat& input);

    Mat getForeground(Mat& input);

private:
    Mat getForegroundMask(Mat& input);
    void removeBackground(Mat& input, Mat& background);

private:
    Mat background;
    bool calibrated;
};