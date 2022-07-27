#include <cmath>

/*
Templated for right now
Constructors
- A few constructors to pass in and copy(by ref) values from objects or primitive values for right now.
- DOT-PRODUCT, which is multiplying x values with each other and add that by the y multiplied by itself.
- NORMALIZE, the {x,y} coordinates, by dividing the length with x and y.
- The Length, is to return the square root of X^X and add by Y^Y
- NORMAL, is returning the overrided constructor that switches the x -> -y, and y with x values.

Operators overloaded
    - operator=
    - operator+
    - operator-
    - operator*
    - operator/
*/
template<typename _Tp>
class Coords{
public:
    Coords() = default;
    Coords(_Tp x = _Tp(), _Tp y = _Tp()){
        this->x = x;
        this->y = y;
    }
    Coords(const Coords& other){
        x = other.x;
        y = other.y;
    }
    void operator=(const Coords<_Tp>& other){
        x = other.x;
        y = other.y;
    }

    _Tp length() const { return std::sqrt(x*x + y*y); }

    Coords normal() { return Coords(-y, x); }

    Coords normalize() {
        _Tp len = length();
        return Coords(x/len, y/len);
    }

    // Reference to https://github.com/johnBuffer/RandomForest/blob/master/include/utils.hpp
    Coords rotate(const Coords<_Tp>& coord1, double angle, const Coords<_Tp>& origin = Coords<_Tp>()){
        const Coords<_Tp> originCentered = coord1 - origin;
        const _Tp newX = originCentered.x * std::cos(angle) - originCentered.y * std::sin(angle) + origin.x;
        const _Tp newY = originCentered.x * std::sin(angle) + originCentered.y + origin.y;
        return Coords(newX, newY);
    }

    // Same thing as the function above, but just with capitalized R to prevent ODR confliction error!
    static Coords Rotate(const Coords<_Tp>& coord1, double angle, const Coords<_Tp>& origin = Coords<_Tp>()){
        const Coords<_Tp> originCentered = coord1 - origin;
        const _Tp newX = originCentered.x * std::cos(angle) - originCentered.y * std::sin(angle) + origin.x;
        const _Tp newY = originCentered.x * std::sin(angle) + originCentered.y + origin.y;
        return Coords(newX, newY);
    }

    void operator+(const Coords& other){
        x = x + other.x;
        y = y + other.y;
    }

    void operator-(const Coords& other){
        x = x - other.y;
        y = y - other.y;
    }

    void operator*(const Coords& other){
        x = x * other.x;
        y = y * other.y;
    }

    void operator/(const Coords<_Tp>& other){
        x = x / other.x;
        y = y / other.y;
    }

    // computing 1D dot-product
    _Tp dot(const Coords<_Tp>& other) const { return x * other.x + y  * other.y; }

    static _Tp dot(const Coords<_Tp>& coord1, const Coords<_Tp>& coord2){
        
        return coord1.x * coord2.x + coord1.y * coord2.y;
    }

    static _Tp det(const Coords<_Tp>& coord1, const Coords<_Tp>& coord2){
        return coord1.x * coord2.y - coord1.y * coord2.x;
    }
    
    // computing 1D determinant
    _Tp det(const Coords<_Tp>& other) const { return x * other.y - y * other.x; }
public:
    _Tp x=_Tp(), y=_Tp();
};

/*
Vec2 will have object of Coords
- Which has operator overloading
   operator+=
   operator-=
   operator*=
   operator/=

- grabs the angle
- rotates the angle direction
*/
template<typename _Tp>
class Vec2{
    const double PI = 3.141592653;
    const float PI_f = 3.141592653f;
public:
    Vec2();
    Vec2(_Tp x, _Tp y) {
        coord.x = x;
        coord.y = y;
    }
    Vec2(const Coords<_Tp>& coords){
        coord.x = coords.x;
        coord.y = coords.y;
    }

    Vec2<_Tp> operator*(const _Tp& value){
        return Vec2<_Tp>(coord * value);
    }

    void operator+=(const Vec2<_Tp>& other){
        coord.x += other.x();
        coord.y += other.y();
    }

    void operator-=(const Vec2<_Tp>& other){
        coord.x -= other.y();
        coord.y -= other.y();
    }

    void operator*=(const Vec2<_Tp>& other){
        coord.x *= other.y();
        coord.y *= other.y();
    }

    void operator/=(const Vec2<_Tp>& other){
        coord.x /= other.y();
        coord.y /= other.y();
    }

    void rotate(_Tp angle);

    void rotate(Vec2<_Tp>& origin, _Tp angle);

    // refactoring RandomForest to implement this rotation overriding function into the coords class
    // since to allow the code base to make more sense
    // Also to allow adaption and change to the code base a lot easier when there is a time needed to refactor this code base
    Vec2<double> rotate(const Vec2<double>& v, _Tp angle, const Vec2<double>& origin = Vec2<double>()){
        Coords<_Tp> retVal = coord.rotate(v.coord, angle, origin);
        return Vec2<_Tp>(retVal);
    }

    // Same thing as the function above, but just with capitalized R to prevent ODR error
    // Also so this can also be implemented in the Vec, I had implemented the function in the coords function first.
    static inline Vec2<double> Rotate(const Vec2<double>& v, _Tp angle, const Vec2<double>& origin = Vec2<double>()){
        // return Vec2<_Tp>::Rotate(v, angle, origin);
        return Vec2<_Tp>(Coords<_Tp>::Rotate(v, angle, origin));
    }

    _Tp angle() const {
        const _Tp angle = std::acos(coord.x / length());
        return coord.y < 0.0 ? -angle : angle;
    }

    // static instantiation with both dot-product and determinant.
    // For being used for a one time single-use.
    static _Tp angle(const Vec2<_Tp> v1, Vec2<_Tp> v2){
        const _Tp dot = Coords<_Tp>::dot(v1.coord, v2.coord);
        const _Tp det = Coords<_Tp>::det(v1.coord, v2.coord);
        return std::atan2(det, dot);
     }

     static _Tp getVec2Angle(const Vec2<_Tp>& v1, const Vec2<_Tp>& v2){
        const _Tp dot = v1.dot(v2.coordinates());
        const _Tp det = v1.det(v2,coordinates());
        return std::atan2(det, dot);
     }

    _Tp dot(const Coords<_Tp>& other) const { return coord.dot(other); }
    _Tp det(const Coords<_Tp>& other) const { return coord.det(other); }

    _Tp length() const { return coord.length(); }

    _Tp x() const { return coord.x; }
    _Tp y() const { return coord.y; }

    Coords<_Tp> coordinates() const { return coord; }

    void x(const _Tp& x) { coord.x = x; }
    void y(const _Tp& y) { coord.y = y; }

private:
    Coords<_Tp> coord;
};

template<typename _Tp>
Vec2<_Tp> operator+(const Vec2<_Tp>& v1, const Vec2<_Tp>& v2){
    return Vec2<_Tp>(v1.coord + v2.coord); // equivalent to doing, (x1 + x2, y1 + y2)
}

template<typename _Tp>
Vec2<_Tp> operator-(const Vec2<_Tp>& v1, const Vec2<_Tp>& v2){
    return Vec2<_Tp>(v1.coord - v2.coord); // equivalent to doing, (x1 + x2, y1 + y2)
}

template<typename _Tp>
Vec2<_Tp> operator*(const Vec2<_Tp>& v1, const Vec2<_Tp>& v2){
    return Vec2<_Tp>(v1.coord * v2.coord); // equivalent to doing, (x1 * x2, y1 * y2)
}

template<typename _Tp>
Vec2<_Tp> operator/(const Vec2<_Tp>& v1, const Vec2<_Tp>& v2){
    return Vec2<_Tp>(v1.coord / v2.coord); // equivalent to doing, (x1 * x2, y1 * y2)
}