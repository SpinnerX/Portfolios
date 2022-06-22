#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;

class SkinDetection{
public:
    SkinDetection();

    void drawSkinColorSampler(Mat& input);
    void calibrate(Mat& input);
    Mat getSkinMask(Mat& input);

private:
    // Helper functions.
    void calculateThreshold(Mat& sample1, Mat& sample2);
    void performOpening(Mat& binaryImg, int structuralElementShade, Point& structuralElementSize);

private:
    int hLowThreshold;
    int hHighThreshold;
    int sLowThreshold;
    int sHighThreshold;
    int vLowThreshold;
    int vHighThreshold;

    bool calibrated;

    Rect skinColorSamplerRectangle1, skinColorSamplerRectangle2;
};