#include <iostream>
#include <memory>
#include "../../Vector/Vec2.h"

template<typename _Tp>
struct Wind{
    Wind(_Tp w, _Tp force, _Tp spd, _Tp start=_Tp()) : width(w), strength(force), speed(spd), pos_x(start) { }

    void update(_Tp dt, _Tp startingX, _Tp endingX){
        pos_x += speed * dt; // updating x position based on speed * delta-time
        if(pos_x - width * 0.5 > endingX) pos_x = startingX - width * 0.5;
    }

    bool isOver(const Vec2<double> position) const{
        return (position.coordinates().x > pos_x-width * 0.5) && (position.coordinates().x < pos_x + width * 0.5);
    }

    _Tp width;
    _Tp strength;
    _Tp pos_x;
    _Tp speed;
};