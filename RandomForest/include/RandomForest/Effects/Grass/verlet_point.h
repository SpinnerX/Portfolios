#include <iostream>
#include <memory>
#include "../../../../Vector/Vec2.h"

/*
First Implement the vertex points to for the grass effets.
*/

class VerletPoint{
    using Ptr = std::shared_ptr<VerletPoint>;
public:
    VerletPoint() : coords(), prevCoords(), accelearation(), mass(1.0), isMoving(true) { }
    VerletPoint(Coords<double> newCoords, double mass) : coords(), prevCoords(newCoords), acceleration(), mass(mass), isMoving(true) { } 
    void rollback(){
        coords = prevCoords;
        acceleration = Vec2<double>(Coords(0.0, 0.0));
    }

    void moveTo(Coords<double>& coords, bool bypass=false){
        if(isMoving or bypass) this->coords = coords;
    }

    void moveTo(const Vec2<double>& point, bool bypass=false){
        if(moving or bypass){
            coords = point.coordinates();
        }
    }

    void moveTo(Coords<double>& coords){
        if(isMoving){
            this->coords += coords;
        }
    }

    void move(const Vec2<double>& point){
        if(isMoving){
            coords += point.coordinates();
        }
    }

    void moveAbs(const Vec2<double>& point){
        if(isMoving){
            coords += point.coordinates();
            prevCoords += point.coordinates();
        }
    }

    void stop(){
        preCoords = coords;
    }

    void update(double dt){
        if(isMoving){
            const Vec2<double> vec = coords - prevCoords;
            acceleration = acceleration - vec * 1.0;
            prevCoords = coords;
            coords += vec + acceleration * (dt * dt / mass);
            acceleration = Vec<double>(0.0, 0.0);
        }
    }

    static Ptr create(Coords<double> coords, double mass){
        return std::make_shared<VerletPoint>(coords, mass);
    }

    void operator=(const Coords<double>& other){
        this->coords = other;
    }

    void operator=(const VerletPoint& other){
        coords = other.coords;
        prevCoords = other.prevCoords;
        acceleration = other.acceleration;
    }

    Coords<double> coords; // grabs current coords
    Coords<double> prevCoords; // grabs previous-given coords
    Vec2<double> acceleration; // Handles the acceleration of the various points.

    double mass;
    bool isMoving;
};

Vec2<double> operator-(const VerletPoint& p1, const VerletPoint& p2){
    return p1.coords - p2.coords;
}