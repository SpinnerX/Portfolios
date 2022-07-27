#include <iostream>
#include <memory>
#include "../../../Vector/Vec2.h"

template<typename _Tp>
class VertexPoint{
public:
    using Ptr = std::shared_ptr<VertexPoint>;
    VertexPoint() : coords(Coords<_Tp>()), prevCoords(Coords<_Tp>()), accelerations(Coords<_Tp>()), mass(_Tp()), isMoving(true) { }
    VertexPoint(Coords<_Tp>& newCoords, _Tp newMass) : coords(newCoords), prevCoords(newCoords), accelerations(Coords<_Tp>()), mass(newMass), isMoving(true) { }
    VertexPoint(Vec2<_Tp>& newCoords, _Tp newMass) : coords(newCoords.coordinates()), prevCoords(newCoords.coordinates()), accelerations(Coords<_Tp>()), mass(newMass), isMoving(true) { }

    void rollback(){
        coords = prevCoords;
        accelerations = Coords<_Tp>(_Tp(), _Tp());
    }

    void moveTo(Coords<_Tp>& coords, bool bypass=false){
        if(isMoving || bypass){ this->coords = coords; }
    }

    void moveTo(const Vec2<_Tp>& point, bool bypass=false){
        if(isMoving || bypass){ coords = point.coordinates(); }
    }

    void move(Coords<_Tp>& coords){
        if(isMoving) this->coords = coords;
    }

    void move(const Vec2<_Tp>& point){
        if(isMoving) this->coords += point.coordinates();
    }

    void moveAbs(const Vec2<_Tp>& point){
        if(isMoving){
            this->coord += point.coordinates();
            this->prevCoords += point.coordinates();
        }
    }

    void stop(){ prevCoords = coords; }

    void update(_Tp dt){
        if(isMoving){
            const Vec2<_Tp> v = coords - prevCoords;

            // Air Friction
            accelerations = accelerations - v / _Tp(1.0);
            prevCoords = coords;
            coords += v + accelerations * (dt*dt / mass);
            accelerations = Vec2<_Tp>(_Tp(), _Tp());
        }
    }

    static Ptr create(Coords<_Tp>& coords, double mass){
        return std::make_shared<VertexPoint>(coords, mass);
    }

    Vec2<double> coords;
    Coords<double> prevCoords;
    Coords<double> accelerations;
    double mass;
    bool isMoving;
};

Vec2<double> operator-(const VertexPoint<double>& p1, const VertexPoint<double> p2){
    // p1.coords - p2.coords -> would work if you only did that with templated objects of Vec2<_Tp> type!
    Vec2<double> coords = p1.coords - p2.coords;
    return Vec2<double>(coords);
}