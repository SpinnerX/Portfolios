#include <iostream>
#include "include/BackgroundParser.h"
#include "include/SkinDetection.h"
#include "include/FaceDetector.h"
#include "include/FingerPoints.h"
// #include "include/Camera.h"
using namespace std;

int main(){
    int cameraType = 0; // (0) is default pc cam
    VideoCapture camera(cameraType);
    bool cameraOn = camera.isOpened();

    if(!cameraOn){
        cout << "Cannot find camera index!" << endl;
        return -1;
    }

    Mat frame, frameOutput, handMask, foreground, fingerPointsDebug;

    BackgroundParser parser = BackgroundParser();
    SkinDetection skinDet = SkinDetection();
    FaceDetector faceDet = FaceDetector();
    FingerPoints fingerPoints = FingerPoints();

    while(cameraOn){
        camera >> frame;

        frameOutput = frame.clone();

        int key = waitKey(1);

        if(key == 27) cameraOn = false; // ESC

        skinDet.drawSkinColorSampler(frameOutput);
        
        foreground = parser.getForeground(frame);

        faceDet.removeFaces(frame, foreground);
        handMask = skinDet.getSkinMask(foreground);
        fingerPointsDebug = fingerPoints.findFingersPoints(handMask, frameOutput); // Very buggy

        parser.calibrate(frame);
        skinDet.calibrate(frame); // Helps, in calibrating how we segment hand gestures
        
        // else if(key == 98) parser.calibrate(frame); // B
        // else if(key == 115) skinDet.calibrate(frame); // S
        imshow("output", frameOutput);
		// imshow("foreground", foreground); // Not working (Screen turns black)
		imshow("handMask", handMask); // Not working (screen turns black)
		imshow("handDetection", fingerPointsDebug); // Not working (screen turns black)
    }

    
    return 0;
}