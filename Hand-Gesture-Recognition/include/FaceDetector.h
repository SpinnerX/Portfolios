#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

class FaceDetector{
public:
    FaceDetector();
    void removeFaces(Mat& input, Mat& output);

private:
    Rect getFaceRect(Mat& input);


    std::string faceClassifierName;
    CascadeClassifier faceCascadeClassifier;
};