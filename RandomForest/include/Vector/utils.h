#include <iostream>
#include <cmath>

/*
NOTE: Don't include
*/

const double PI = 3.141592653;
const float PI_f = 3.141592653f;

/*
// Refactored: Adding this into the Vec2 class as a static function.
double getVec2Angle(const Vec2<double>& v1, const Vec2<double>& v2){
    const double dot = v1.dot(v2.coordinates());
    const double det = v1.det(v2.coordinates());
    return std::atan2(det, dot);
}*/
/*
// Refactored to add this function as a static in the Number Generator class instantiation
double getRandRange(double width){
    constexpr int32_t maxValue = 10000;
    const int32_t half = maxValue / 2;
    constexpr float div = 1.0 / double(maxValue);

    const double value = (rand() % maxValue - half) * div * width;
    return val;
}*/

/*
double getRandUnder(double max){
    constexpr int32_t max_value = 10000;
	constexpr int32_t half = max_value / 2;
	constexpr float div = 1.0f / float(max_value);
	const float val = (rand() % max_value) * div * max;
	return val;
}
*/

/*
double getLength(const Vec2<double>& vec){
    return vec.length();
}*/

/*
Vec2<double> rotate(const Vec2<double> v, double angle, const Vec2<double> origin = Vec2<double>(0.0, 0.0)){
    // Allows for a single-use instatiation using static.
    // Deleting this because or redundancy...
    return Vec2<double>::Rotate(v, angle, origin);
}*/

double sign(double a){
    return a < 0.0 ? -1.0 : 1.0;
}