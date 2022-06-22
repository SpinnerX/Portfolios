#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
using namespace cv;

class FingerPoints{
public:
    FingerPoints();

    Mat findFingersPoints(Mat& inputImage, Mat& frame);

private:
    double findingPointsDistance(Point& a, Point& b);
    std::vector<Point> compactOnNeighborMedian(std::vector<Point>& points, double maxNeighborDist);

    double findAngle(Point& a, Point& b, Point& c);
    
    bool isFinger(Point& a, Point& b, Point& c, double limit_angle_inf, double limit_angle_sup, cv::Point& palmCenter, double distance_from_palm_tollerance);
    
    std::vector<Point> findNearestOnX(std::vector<Point>& points, Point& pivot);

    double findPointDistanceOnX(Point& a, Point& b);
    
    void drawVectorPoints(Mat& image, std::vector<Point>& points, Scalar& color, bool with_numbers);

private:
    Scalar color_blue;
	Scalar color_green;
	Scalar color_red;
	Scalar color_black;
	Scalar color_white;
	Scalar color_yellow;
	Scalar color_purple;
};