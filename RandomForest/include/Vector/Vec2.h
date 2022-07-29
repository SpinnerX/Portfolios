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
    Coords(){
        x = _Tp();
        y = _Tp();
    }
    Coords(_Tp x=_Tp(), _Tp y=_Tp()){
        this->x = x;
        this->y = y;
    }
    Coords(const Coords<_Tp>& other){
        x = other.x;
        y = other.y;
    }

    _Tp angle() const{
        const _Tp angle = std::acos(x / length());
        return y < 0.0 ? -angle : angle;
    }

    static _Tp angle(const Coords<_Tp>& coord1, const Coords<_Tp>& coord2){
        const _Tp dot = Coords<_Tp>::dot(coord1. coord2);
        const _Tp det = Coords<_Tp>::det(coord1, coord2);
        return std::atan2(det, dot);
    }

    _Tp length() const { return std::sqrt(x*x + y*y); }

    Coords<_Tp> normal(){ return Coords<_Tp>(-y, x); }

    Coords<_Tp> normalize(){
        _Tp len = this->length();
        return Coords<_Tp>(x/len, y/len);
    }

    // Computing 1D dot-product
    _Tp dot(const Coords<_Tp>& other){ return this->x * other.x + y * other.y; }

    static _Tp dot(const Coords<_Tp>& coord1, const Coords<_Tp>& coord2) { return coord1.x * coord2.x + coord1.y * coord2.y; }

    // computing 1D determinant
    _Tp det(const Coords<_Tp>& other){ return x * other.x - y  * other.y; }

    static _Tp det(const Coords<_Tp>& coord1, const Coords<_Tp>& coord2) { return coord1.x * coord2.x - coord1.y * coord2.y; }

    // Rotation
    Coords<_Tp> rotate(const Coords<_Tp>& coord1, _Tp angle, const Coords<_Tp>& origin = Coords<_Tp>()){
        const Coords<_Tp> originCentered = coord1 - origin;
        const _Tp newX = originCentered.x * std::cos(angle) - originCentered.y * std::sin(angle) + origin.x;
        const _Tp newY = originCentered.x * std::sin(angle) + originCentered.y + origin.y;

        return Coords<_Tp>(newX, newY);
    }

    static Coords<_Tp> Rotate(const Coords<_Tp>& coord1, _Tp angle, const Coords<_Tp>& origin = Coords<_Tp>()){
        const Coords<_Tp> originCentered = coord1 - origin;
        const _Tp newX = originCentered.x * std::cos(angle) - originCentered.y * std::sin(angle) + origin.x;
        const _Tp newY = originCentered.x * std::sin(angle) + originCentered.y + origin.y;

        return Coords<_Tp>(newX, newY);
    }

    Coords<_Tp> operator+(const Coords<_Tp>& other){
        _Tp newX = x + other.x;
        _Tp newY = y + other.y;
        return Coords<_Tp>(newX, newY);
    }

    Coords<_Tp> operator+=(const Coords<_Tp>& other){
        this->x += other.x;
        this->y += other.y;
        return Coords<_Tp>(x, y);
    }

    Coords<_Tp> operator-(const Coords<_Tp>& other){
        _Tp newX = x - other.x;
        _Tp newY = x - other.y;
        return Coords<_Tp>(newX, newY);
    }

    Coords<_Tp> operator-=(const Coords<_Tp>& other){
        this->x -= other.x;
        this->y = other.y;
        return Coords<_Tp>(x, y);
    }

    Coords<_Tp> operator*(const Coords<_Tp>& other){
        _Tp newX = x * other.x;
        _Tp newY = x * other.y;
        return Coords<_Tp>(newX, newY);
    }

    Coords<_Tp> operator*=(const Coords<_Tp>& other){
        this->x *= other.x;
        this->y *= other.y;
        return Coords<_Tp>(x, y);
    }

    Coords<_Tp> operator/(const Coords<_Tp>& other){
        _Tp newX = x / other.x;
        _Tp newY = x / other.y;
        return Coords<_Tp>(newX, newY);
    }

    Coords<_Tp> operator/=(const Coords<_Tp>& other){
        this->x /= other.x;
        this->y /= other.y;
        return Coords<_Tp>(x, y);
    }

    void operator=(const Coords<_Tp>& other){
        this->x = other.x;
        this->y = other.y;
    }
public:
    _Tp x = _Tp(), y = _Tp();
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
public:
    Vec2(){
        coords = Coords<_Tp>();
    }
    Vec2(_Tp x, _Tp y){
        coords = Coords<_Tp>(x, y);
    }
    Vec2(const Coords<_Tp>& other){
        coords.x = other.x;
        coords.y = other.y;
    }

    void rotate(const _Tp& angle){ rotate(Vec2<_Tp>(), angle); }

    void rotate(const Vec2<_Tp>& origin, _Tp angle){
        const Vec2<_Tp> v = Vec2<_Tp>(coords-origin);
        const _Tp ca = std::cos(angle);
        const _Tp sa = std::sin(angle);

        this->x = (ca * v.x() - sa * v.y());
        this->y = (sa * v.x() - ca * v.y());
    }

    // refactoring RandomForest to implement this rotation overriding function into the coords class
    // since to allow the code base to make more sense
    // Also to allow adaption and change to the code base a lot easier when there is a time needed to refactor this code base
    Vec2<_Tp> rotate(const Vec2<_Tp>& v, _Tp angle, const Vec2<_Tp>* origin = Vec2<_Tp>()){
        Coords<_Tp> retVal = coords.rotate(v.coords, angle, origin);
        return retVal;
    }

    static Vec2<_Tp> Rotate(const Vec2<_Tp>& v, _Tp angle, const Vec2<_Tp>* origin = Vec2<_Tp>()){
        return Coords<_Tp>::rotate(v, angle, origin);
    }
    
    _Tp angle() const {
        const _Tp angle = std::acos(coords.x);
    }

    inline _Tp angle(const Vec2<_Tp>& v1, const Vec2<_Tp>& v2){
        return Coords<_Tp>::angle(v1, v2);
    }

    static _Tp Angle(const Vec2<_Tp>& v1, const Vec2<_Tp>& v2){
        return Coords<_Tp>::angle(v1.coords, v2.coords);
    }

    void operator+=(const Vec2<_Tp>& other){
        coords += other;
    }

    void operator-=(const Vec2<_Tp>& other){
        coords -= other.coords;
    }

    Vec2<_Tp> operator*(const _Tp& value){ return Vec2<_Tp>(coords * value); }

    void operator*=(const Vec2<_Tp>& other){
        coords *= other.coords;
    }

    void operator/=(const Vec2<_Tp>& other){
        coords /= other.coords;
    }

    Coords<_Tp> coordinates() const { return coords; }

    _Tp x() const { return coords.x; }
    _Tp y() const { return coords.y; }

    void x(const _Tp& newX) { coords.x = newX; }
    void y(const _Tp& newY) { coords.y = newY; }

    _Tp length() { return coords.length(); }

    static _Tp length(const Vec2<_Tp>& v) { return v.length(); }

private:
    Coords<_Tp> coords;
};

template<typename _Tp>
Vec2<_Tp> operator+(const Vec2<_Tp>& v1, const Vec2<_Tp>& v2){
    return Vec2<_Tp>(v1.coordinates() + v2.coordinates());
}

template<typename _Tp>
Vec2<_Tp> operator-(const Vec2<_Tp>& v1, const Vec2<_Tp>& v2){
    return Vec2<_Tp>(v1.coordinates() - v2.coordinates());
}

template<typename _Tp>
Vec2<_Tp> operator*(const Vec2<_Tp>& v1, const Vec2<_Tp>& v2){
    return Vec2<_Tp>(v1.coordinates() * v2.coordinates());
}

template<typename _Tp>
Vec2<_Tp> operator*(const Vec2<_Tp>& v1, const _Tp value){
    return Vec2<_Tp>(v1*value);
}

template<typename _Tp>
Vec2<_Tp> operator*(const Coords<_Tp>& coord, const _Tp& v){
    return Vec2<_Tp>(coord * v);
}

template<typename _Tp>
Vec2<_Tp> operator/(const Vec2<_Tp>& v1, const Vec2<_Tp>& v2){
    return Vec2<_Tp>(v1.coordinates() / v2.coordinates());
}
