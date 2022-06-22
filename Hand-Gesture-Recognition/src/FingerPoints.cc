#include "../include/FingerPoints.h"
#include <iostream>
using namespace std;

#define LIMIT_ANGLE_SUP 60
#define LIMIT_ANGLE_INF 5
#define BOUNDING_RECT_FINGER_SIZE_SCALING 0.3
#define BOUNDING_RECT_NEIGHBOR_DISTANCE_SCALING 0.05

FingerPoints::FingerPoints(){
    color_blue = Scalar(255, 0, 0);
	color_green = Scalar(0, 255, 0);
	color_red = Scalar(0, 0, 255);
	color_black = Scalar(0, 0, 0);
	color_white = Scalar(255, 255, 255);
	color_yellow = Scalar(0, 255, 255);
	color_purple = Scalar(255, 0, 255);
}

Mat FingerPoints::findFingersPoints(Mat& input, Mat& frame){
    Mat contouredImage = Mat::zeros(input.size(), CV_8UC3);

    // Checks if the source image is good
    if(input.empty()) return contouredImage;

    // working on the 1 channel output, since the function is called inside is a loop we are not sure that is the always
    if(input.channels() != 1) return contouredImage;

    vector<vector<Point> > contours;
    vector<Vec4i> heirarchy;

    findContours(input, contours, heirarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    // We need at least one contour to work
    // Checking our contour size (bounds-checking)
    if(contours.size() == 1) return contouredImage;

    // finding the biggest contour (assumming its our hand)
    int biggestContourIndex = -1;
    double biggestArea = 0.0;

    for(size_t i = 0; i < contours.size(); i++){
        double area = cv::contourArea(contours.at(i), false);

        if(area > biggestArea){
            biggestArea = area;
            biggestContourIndex = i;
        }
    }

    if(biggestContourIndex < 0) return contouredImage;

    // finding convex hull of objects for each contour, and the defects, two diff data structs needed by OpenCV API
    vector<Point> hullPoints;
    vector<int> hullIntegers;

    // drawing the convexhull and for finding the bounding rectangle.
    Mat temp = Mat(contours.at(biggestContourIndex));
    convexHull(temp, hullPoints, true);

    // for finding the points the convex hull is drawing around the bounded rectangle.
    convexHull(temp, hullIntegers, false);

    // Need at least 3 points, to find the defects
    vector<Vec4i> defects;

    if(hullPoints.size() > 3) convexityDefects(temp, hullIntegers, defects);
    // else return contouredImage;

    // We bound the convex hull
    Mat temp2 = Mat(hullPoints);
    Rect boundingRectangle = boundingRect(temp2);

    // Finding center of bounding rectangles, this should approximately also be center of the hand
    int newX = (boundingRectangle.tl().x + boundingRectangle.br().x)/2;
    int newY = (boundingRectangle.tl().y + boundingRectangle.br().y)/2;
    Point centerBoundingRect(newX, newY);
    // Point centerBoundingRect(newX/2, newY/2);

    // seperate the defects keeping only the ones of interest
    vector<Point> startingPoints;
    vector<Point> furtherPoints;

    for(size_t i = 0; i < defects.size(); i++){
        // Grabs a point based on the contours largest index found.
        Point temp = contours[biggestContourIndex][defects[i].val[0]];
        startingPoints.push_back(temp);

        // startingPoints.push_back(contours.at(biggestContourIndex).at(defects.at(i).val[0]));

        // filtering the furthest point based on its distance from the center of the triangle.
        // Point a = contours.at(biggestContourIndex).at(defects.at(i).val[2]);
        Point a = contours[biggestContourIndex][defects[i][2]];
        Point b = centerBoundingRect;
        
        int height = boundingRectangle.height*BOUNDING_RECT_FINGER_SIZE_SCALING;

        if(findingPointsDistance(a, b) < height){
            // Point point = contours.at(biggestContourIndex).at(defects.at(i).val[2]);
            Point point = contours[biggestContourIndex][defects[i].val[2]];
            furtherPoints.push_back(point);
        }
    }

    // we compact them on their medians
    int height = boundingRectangle.height*BOUNDING_RECT_FINGER_SIZE_SCALING;
    vector<Point> filteredStartingPoints = compactOnNeighborMedian(startingPoints, height);
    vector<Point> filteredFurthestPoints = compactOnNeighborMedian(furtherPoints, height);

    // we try to find the finger points
    vector<Point> filteredFingerPoints;

    if(filteredFurthestPoints.size() > 1){
        vector<Point> fingerPoints;

        for(size_t i = 0; i < filteredStartingPoints.size(); i++){
            // Finding the nearest points.
            vector<Point> closestPoints = findNearestOnX(filteredFurthestPoints, filteredStartingPoints.at(i));

            if(isFinger(closestPoints.at(0), filteredStartingPoints.at(i), closestPoints.at(1), LIMIT_ANGLE_INF, LIMIT_ANGLE_SUP, centerBoundingRect, height))
                fingerPoints.push_back(filteredStartingPoints.at(i));
        }

        if(fingerPoints.size() > 0){
            while(fingerPoints.size() >= 5) fingerPoints.pop_back();

            for(size_t i = 0; i < fingerPoints.size()-1; i++){
                if(findPointDistanceOnX(fingerPoints.at(i), fingerPoints.at(i+1)) > height*1.5){
                    Point temp = fingerPoints[i];
                    filteredFingerPoints.push_back(temp);
                    // filteredFingerPoints.push_back(fingerPoints.at(i));
                }
            }

            if(fingerPoints.size() > 2){
                int newHeight = height*1.5;
                if(findPointDistanceOnX(fingerPoints.at(0), fingerPoints.at(fingerPoints.size()-1)) > height*1.5)
                    filteredFingerPoints.push_back(fingerPoints.at(fingerPoints.size()-1));
            }
            else
                filteredFingerPoints.push_back(fingerPoints.at(fingerPoints.size()-1));
        }
    }

    // draw what we detect
    drawContours(contouredImage, contours, biggestContourIndex, color_green, 2, 8, heirarchy);
    polylines(contouredImage, hullPoints, true, color_blue);
    rectangle(contouredImage, boundingRectangle.tl(), boundingRectangle.br(), color_red, 2, 8, 0);
    circle(contouredImage, centerBoundingRect, 5, color_purple, 2, 8);
    drawVectorPoints(contouredImage, filteredStartingPoints, color_red, true);
    drawVectorPoints(contouredImage, filteredFurthestPoints, color_red, true);
    drawVectorPoints(contouredImage, filteredFingerPoints, color_yellow, false);
    putText(contouredImage, to_string(filteredFingerPoints.size()), centerBoundingRect, FONT_HERSHEY_PLAIN, 3, color_purple);

    // and on the starting frame.

    drawContours(frame, contours, biggestContourIndex, color_green, 2, 8, heirarchy);
    circle(frame, centerBoundingRect, 5, color_purple, 2, 8);
    drawVectorPoints(frame, filteredFingerPoints, color_yellow, false);
    putText(frame, to_string(filteredFingerPoints.size()), centerBoundingRect, FONT_HERSHEY_PLAIN, 3, color_purple);

    return contouredImage;
}

double FingerPoints::findingPointsDistance(Point& a, Point& b){
    Point difference = a - b;
    return sqrt(difference.ddot(difference));
}

vector<Point> FingerPoints::compactOnNeighborMedian(vector<Point>& points, double maxNeighborDistance){
    vector<Point> medianPoints;

    if(points.size() == 0) return medianPoints;

    if(maxNeighborDistance <= 0) return medianPoints;

    // We starting with the first point
    Point reference = points.at(0);
    Point median = points.at(0);

    for(size_t i = 0; i < points.size(); i++){
        if(findingPointsDistance(reference, points.at(i)) > maxNeighborDistance){
            
            // points is not in range, so we save the median.

            medianPoints.push_back(median);

            // swapping references
            reference = points.at(i);
            median = points.at(i);
        }
        else median = (points.at(i) + median) / 2;
    }
    medianPoints.push_back(median);
    
    return medianPoints;
}

double FingerPoints::findAngle(Point& a, Point& b, Point& c){
    double ab = findingPointsDistance(a, b);
    double bc = findingPointsDistance(b, c);
    double ac = findingPointsDistance(a, c);

    return acos((ab * ab + bc - ac * ac) / (2 * ab * ac)) * 180 / CV_PI;
}

bool FingerPoints::isFinger(Point& a, Point& b, Point& c, double limit_angle_inf, double limit_angle_sup, Point& palm_center, double min_distance_from_palm){
    double angle = findAngle(a, b, c);

    if(angle > limit_angle_sup or angle < limit_angle_inf) return false;

    // finger points should be under the far two points.
    int delta_y1 = b.y - a.y;
    int delta_y2 = b.y - c.y;

    if(delta_y1 > 0 and delta_y2 > 0) return false;

    // Two far points should not be both under the center of the hand
    int delta_y3 = palm_center.y - a.y;
    int delta_y4 = palm_center.y = c.y;

    if(delta_y3 < 0 and delta_y4 < 0) return false;

    double distanceFromPalm = findingPointsDistance(b, palm_center);

    if(distanceFromPalm < min_distance_from_palm) return false;

    double distanceFromPalm1 = findingPointsDistance(a, palm_center);
    double distanceFromPalm2 = findingPointsDistance(c, palm_center);

    if(distanceFromPalm1 < min_distance_from_palm/4 or distanceFromPalm2 < min_distance_from_palm/4) return false;

    return true;
}

vector<Point> FingerPoints::findNearestOnX(vector<Point>& points, Point& pivot){
    vector<Point> retVal(2);

    if(points.size() == 0) return retVal;

    double distance_x1 = DBL_MAX;
    double distance_1 = DBL_MAX;
    double distance_x2 = DBL_MAX;
    double distance_2 = DBL_MAX;
    int indexFound = 0;

    for(size_t i = 0; i < points.size(); i++){
        double dist_x = findPointDistanceOnX(pivot, points.at(i));
        double dist = findingPointsDistance(pivot, points.at(i));

        if(dist_x < distance_x1 and dist_x != 0 and dist <= distance_1){
            distance_x1 = dist_x;
            distance_1 = dist;
            indexFound = i;
        }
    }
    retVal.at(0) = points.at(indexFound);

    for(size_t i = 0; i < points.size(); i++){
        double dist_x = findPointDistanceOnX(pivot, points.at(i));
        double dist = findingPointsDistance(pivot, points.at(i));

        if(dist_x < distance_x2 and dist_x != 0 and dist <= distance_2 and  dist_x != distance_x1){
            distance_x2 = dist_x;
            distance_2 = dist;
            indexFound = i;
        }
    }

    retVal.at(1) = points.at(indexFound);

    return retVal;
}

double FingerPoints::findPointDistanceOnX(Point& a, Point& b){
    double retVal = 0.0;

    if(a.x > b.x) retVal = a.x - b.x;
    else retVal = b.x - a.x;

    return retVal;
}

void FingerPoints::drawVectorPoints(Mat& image, vector<Point>& points, Scalar& color, bool withNumbers){
    for(size_t i = 0; i < points.size(); i++){
        circle(image, points.at(i), 5, color, 2, 8);

        if(withNumbers)
            putText(image, to_string(i), points.at(i), FONT_HERSHEY_PLAIN, 3, color);
    }
}