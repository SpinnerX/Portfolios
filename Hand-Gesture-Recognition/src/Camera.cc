#include "../include/Camera.h"
using namespace std;

Camera::Camera(){
    cameraType = 0;
    parser = BackgroundParser();
    skinDet = SkinDetection();
    faceDet = FaceDetector();
    fingerPoints = FingerPoints();
}

void Camera::calibrate(Mat& root){
    VideoCapture camera(cameraType);
    bool cameraOpen = camera.isOpened();

    if(!cameraOpen){
        cout << "Cannot open camera index!" << endl;
        return;
    }

    while(cameraOpen){
        camera >> root;

        Mat output = root.clone();

        int key = waitKey(1);

        if(key == 27) cameraOpen = false;

        skinDet.drawSkinColorSampler(output);
        Mat foreground = parser.getForeground(output);

        faceDet.removeFaces(output, foreground);
        
        Mat handMask = skinDet.getSkinMask(foreground);
        Mat points = fingerPoints.findFingersPoints(handMask, output);

        parser.calibrate(root);
        skinDet.calibrate(root);
        // else if(key == 98) parser.calibrate(frame); // B
        // else if(key == 115) skinDet.calibrate(frame); // S
        imshow("output", output);
		// imshow("foreground", foreground); // Not working (Screen turns black)
		imshow("handMask", handMask); // Not working (screen turns black)
		// imshow("handDetection", fingerPointsDebug); // Not working (screen turns black)
    }
    
}

void Camera::operator()(Mat& cam){
    calibrate(cam);
}