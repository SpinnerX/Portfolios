#include "../include/FaceDetector.h"
using namespace std;

FaceDetector::FaceDetector(){
    // faceClassifierName = "haarcascade_frontalface_alt.xml";
    faceClassifierName = "haarcascade_frontalface_alt.xml";
    if(!faceCascadeClassifier.load(faceClassifierName)) throw runtime_error("Can't load file " + faceClassifierName);
}

void FaceDetector::removeFaces(Mat& input, Mat& output){
    vector<Rect> faces;
    Mat frameGray;

    cvtColor(input, frameGray, COLOR_BGR2GRAY);
    equalizeHist(frameGray, frameGray);

    faceCascadeClassifier.detectMultiScale(frameGray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(120, 120));

    for(size_t i = 0; i < faces.size(); i++){
        rectangle(
            output,
            Point(faces.at(i).x, faces.at(i).y),
            Point(faces.at(i).x+faces.at(i).width, faces.at(i).y + faces.at(i).height),
            Scalar(0,0,0),
            -1
        );
    }
}

Rect FaceDetector::getFaceRect(Mat& input){
    vector<Rect> faceRectangles;
    Mat inputGray;

    cvtColor(input, inputGray, COLOR_BGR2GRAY);
    equalizeHist(inputGray, inputGray);

    faceCascadeClassifier.detectMultiScale(inputGray, faceRectangles, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(120, 120));

    if(faceRectangles.size() > 0) return faceRectangles.at(0);
    else return Rect(0,0,1,1);
}