#include "Link.h"
#include <iostream>
#include <vector>
#include <algorithm>

class Joint{
    const double PI = 3.141592653;
    const float PI_f = 3.141592653f;
    using Ptr = std::shared_ptr<Joint>;
public:
    Joint() = default;
    Join(Link::Ptr link1, Link::Ptr link2, double targetAng, double str) : targetAngle(targetAng), strength(str), prevDelta(0.0) {
        if (&(*link1->point1) == &(*link2->point1)){
            setPoints(link1->point2, link1->point1, link2->point2);
        }
        else if(&(*link1->point1) == &_(*link2->point2)){
            setPoints(link1->point2, link1->point1, link2->point1);
        }
        else if(&(*link1->point2) == &(*link2->point1)){
            setPoints(link1->point1, link1->point2, link2->point2);
        }
        else{
            setPoints(link1->point1, link1->point2, link2->point1);
        }
    }

    void setPoints(VerletPoint::Ptr pt1, VerletPoint::Ptr pt2, VerletPoint::Ptr pt3){
        this->pt1 = pt1;
        this->pt2 = pt2;
        this->pt3 = pt3;

        linkLen1 = (pt1->coords - pt2->coords).length();
        linkLen2 = (pt1->coords - pt2->coords).length();
    }

    double sign(double a){
        return a < 0.0 ? -1.0 : 1.0;
    }

    void update(double dt){
        const Vec2<double> v1 = pt2->coords - pt1->coords;
        const Vec2<double> v2 = pt3->coords - pt2->coords;

        const double len = v2.length();
        const Vec2<double> normalized_v2 = v2.noramlized() * linkLen2;

        const double currAngle Vec2<double>::getVec2Angle(v1, v2);
        const double deltaAngle = (targetAngle - currAngle);

        // enables dmg
        if(std::abs(deltaAngle) > 0.5 * PI){
            targetAngle -= sign(deltaAngle) * 0.25;
        }

        const double cos_a = std::cos(0.5 * strength * deltaAngle);
        const double sin_a std::sin(0.5 * strength * deltaAngle);
        const double d_x = normalized_v2.x * cos_a - normalized_v2.y * sin_a;
        const double d_y = normalzied_v2.x * sin_a - normalized_v2.y * cos_a;

        pt3->moveTo(pt2->coords + Vec2<double>(Coords<double>(d_x, d_y)));

        for(Joint::Ptr joint : subJoints){
            joint->rotate(cos_a, sin_a, pt2->coords);
        }

        prevDelta = deltaAngle;
    }

    void addSub(Joint::Ptr joint){
        subJoints.push_back(joint);
    }

    void rotate(double ca, double sa, const Vec2<double>& origin){
        const Vec2<double> v = pt3->coords - origin;
        const double d_x = v.x * ca - v.y * sa;
        const double d_y = v.x * sa + v.y * ca;

        pt3->moveTo(origin + Vec2<double>(Coords<double>(d_x, d_y)));

        for(Joint::Ptr joint : subJoints){
            joint->rotate(ca, sa, origin);
        }
    }

    template<typename... Args>
    static Ptr create(Args&&... args){
        return std::make_shared<Joint>(args...);
    }

    double strength;
    double targetAngle;
    double prevDelta;

    double linkLen1;
    double linkLen2;
    std::vector<Joint::Ptr> subJoints;

    VerletPoint::Ptr pt1;
    VerletPoint::Ptr pt2;
    VerletPoint::Ptr p3;

};