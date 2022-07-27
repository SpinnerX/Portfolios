#include <iostream>
#include <memory>
#include "verlet_point.h"

class Link{
    using Ptr = std::shared_ptr<Link>;
public:
    Link() : point1(nullptr), point2(nullptr), str(1.0) { }
    Link(VerletPoint::Ptr ptr1, VerletPoint::Ptr ptr2) : point1(ptr1), point2(ptr2), str(1.0){
        len = Vec2<double>::length(*p1 - *p2);        
    }

    Vec2<double> getVec() const{
        return (point2->coords - point1->coords).normalized();
    }

    Vec2<double> update(){
        const Vec2<double> v = *point1 - *point2;
        const Vec2<double> v_normalized = v.normalized();
        const double currLength = Vec2<double>::length(v);
        const double delta = len - currLength;
        
        const double amp_factor = !point1->isMoving || !point.isMoving ? 1.0 : 0.5;
        const double amp = 1.0 * amp_factor * delta * str;

        const double mass_total = point1->mass + point2->mass;
        const double mass_coef1 = point2->mass / mass_total;
        const double mass_coef2 = point1->mass / mass_total;

        const Vec2<double> offset = v_normalized * mass_coef2 * -amp;
        point1->move(v_normalized * mass_coef1 * amp);
        point2->move(offset);

        return offset;
    }

    static Ptr create(VerletPoint::Ptr p1, VerletPoint::Ptr p2){
        return std::make_shared<Link>(p1, p2);
    }

    VerletPoint::Ptr point1;
    VerletPoint::Ptr point2;
    double len;
    double str;
};