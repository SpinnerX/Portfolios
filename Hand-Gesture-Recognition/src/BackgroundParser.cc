#include "../include/BackgroundParser.h"
#include <iostream>
using namespace std;

BackgroundParser::BackgroundParser(){
    background = Mat();
    calibrated = false;
}

void BackgroundParser::calibrate(Mat& input){
    cvtColor(input, background, COLOR_BGR2GRAY);
    calibrated = true;
}

Mat BackgroundParser::getForeground(Mat& input){
    Mat foregroundMask = getForegroundMask(input);

    Mat foreground;
    input.copyTo(foreground, foregroundMask);

    return foreground;    
}

Mat BackgroundParser::getForegroundMask(Mat& input){
    Mat foregrountMask;

    if(!calibrated){
        foregrountMask = Mat::zeros(input.size(), CV_8UC1);
        return foregrountMask;
    }

    cvtColor(input, foregrountMask, COLOR_BGR2GRAY);
    removeBackground(foregrountMask, background);

    return foregrountMask;
}

void BackgroundParser::removeBackground(Mat& input, Mat& background){
    int thresholdOffset = 10;

    for(int i = 0; i < input.rows; i++){
        for(int j = 0; j < input.cols; j++){
            uchar framePixel = input.at<uchar>(i,j);
            uchar bgPixel = background.at<uchar>(i,j);

            if(framePixel >= bgPixel-thresholdOffset and framePixel <= bgPixel+thresholdOffset) input.at<uchar>(i,j) = 0;
            else input.at<uchar>(i,j) = 255;
        }
    }
}