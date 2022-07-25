#include <iostream>
#include <cstring>
#include <cstdint>
#include <memory>
#include "../Vector/Vec2.h"

class Node_Conf{
public:
    Node_Conf();
    Node_Conf(const Vec2<double>& position){

    }
    Node_Conf(const Vec2<double>& position, const Vec2<double>& direction, double len, double w, uint32_t lvl, uint32_t i);

    Vec2<double> pos() const { return _pos; }
    Vec2<double> growth() const { return _growth; }
    double len() const { return length; }
    double width() const { return w; }
    double lvl() const { return level; }
    double index() const { return indx; }
    bool isDone() const { return _isDone; }

    Vec2<double> getEnd(){
        return _pos *  (_growth * length);
    }

    Vec2<double> getEnd(Vec2<double>& other){
        return other + (_growth * length);
    }

    void operator=(const Node_Conf& other){
        this->_pos = other._pos;
        this->_growth = other._growth;
        this->length = other.length;
        this->level = other.level;
        this->indx = other.indx;
        this->_isDone = other._isDone;
    }

private:
    Vec2<double> _pos;
    Vec2<double> _growth;
    double length;
    double w;
    uint32_t level;
    uint32_t indx; // index
    bool _isDone;
};