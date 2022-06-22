#include "../include/SkinDetection.h"
using namespace std;

SkinDetection::SkinDetection(){
    hLowThreshold = 0;
	hHighThreshold = 0;
	sLowThreshold = 0;
	sHighThreshold = 0;
	vLowThreshold = 0;
	vHighThreshold = 0;

	calibrated = false;

    skinColorSamplerRectangle1 = Rect();
    skinColorSamplerRectangle2 = Rect();
}

void SkinDetection::drawSkinColorSampler(Mat& input){
    int frameWidth = input.size().width, frameHeight = input.size().height;
    int rectSize = 20;
    Scalar rectangleColor = Scalar(255, 0, 255);

    skinColorSamplerRectangle1 = Rect(frameWidth/5, frameHeight/2, rectSize, rectSize);
    skinColorSamplerRectangle2 = Rect(frameWidth/5, frameHeight/3, rectSize, rectSize);

    rectangle(
        input,
        skinColorSamplerRectangle1,
        rectangleColor
    );

    rectangle(
        input,
        skinColorSamplerRectangle2,
        rectangleColor
    );
}

void SkinDetection::calibrate(Mat& input){
    Mat hsvInput;
    cvtColor(input, hsvInput, COLOR_BGR2HSV);

    Mat sample1 = Mat(hsvInput, skinColorSamplerRectangle1);
    Mat sample2 = Mat(hsvInput, skinColorSamplerRectangle2);

    calculateThreshold(sample1, sample2);

    calibrated = true;
}

/*
Threshold Calculation

*/
void SkinDetection::calculateThreshold(Mat& sample1, Mat& sample2){
    // int offsetLowThreshold = 80;
    // int offsetHighThreshold = 30;
    // int offsetLowThreshold = 30;
    // int offsetHighThreshold = 80;
    int offsetLowThreshold = 30;
    int offsetHighThreshold = 80;

    Scalar hsvMeansSample1 = mean(sample1);
    Scalar hsvMeansSample2 = mean(sample2);

    /*
    cout << "HSV  Mean Sample 1 [0]: " <<  hsvMeansSample1[0] << endl;
    cout << "HSV Mean Sample 1 [1]: " << hsvMeansSample1[1] << endl;
    cout << "HSV Mean Sample 1 [2]: " << hsvMeansSample1[2] << endl;

    cout << "HSV  Mean Sample 2 [0]: " <<  hsvMeansSample2[0] << endl;
    cout << "HSV Mean Sample 2 [1]: " << hsvMeansSample2[1] << endl;
    cout << "HSV Mean Sample 2 [2]: " << hsvMeansSample2[2] << endl;
    */

    hLowThreshold = min(hsvMeansSample1[0], hsvMeansSample2[0] - offsetLowThreshold);
    hHighThreshold = max(hsvMeansSample1[0], hsvMeansSample2[0] + offsetLowThreshold);

    sLowThreshold = min(hsvMeansSample1[1], hsvMeansSample2[1]) - offsetLowThreshold;
	sHighThreshold = max(hsvMeansSample1[1], hsvMeansSample2[1]) + offsetHighThreshold;

	// the V channel shouldn't be used. By ignorint it, shadows on the hand wouldn't interfire with segmentation.
	// Unfortunately there's a bug somewhere and not using the V channel causes some problem. This shouldn't be too hard to fix.
	// vLowThreshold = min(hsvMeansSample1[2], hsvMeansSample2[2]) - offsetLowThreshold;
	// vHighThreshold = max(hsvMeansSample1[2], hsvMeansSample2[2]) + offsetHighThreshold;
    vLowThreshold = min(hsvMeansSample1[2], hsvMeansSample2[2] - offsetLowThreshold);
	vHighThreshold = max(hsvMeansSample1[2], hsvMeansSample2[2] + offsetHighThreshold);
}

Mat SkinDetection::getSkinMask(Mat& input){
    Mat skinMask;

    if(!calibrated){
        skinMask = Mat::zeros(input.size(), CV_8UC1);
        return skinMask;
    }

    Mat hsvInput;
    cvtColor(input, hsvInput, COLOR_BGR2HSV);

    inRange(
        hsvInput,
        Scalar(hLowThreshold, sLowThreshold, vLowThreshold),
        Scalar(hHighThreshold, sHighThreshold, vHighThreshold),
        skinMask
    );

    Point point = Point(3,3);
    // performOpening(skinMask, MORPH_ELLIPSE, {3,3});
    performOpening(skinMask, MORPH_ELLIPSE, point);
    dilate(skinMask, skinMask, Mat(), Point(-1, -1), 3);

    return skinMask;
}

void SkinDetection::performOpening(Mat& binaryImg, int kernelShape, Point& kernelSize){
    Mat struicturalElement = getStructuringElement(kernelShape, kernelSize);
    morphologyEx(binaryImg, binaryImg, MORPH_OPEN, struicturalElement);
}