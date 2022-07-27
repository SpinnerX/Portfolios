#include <iostream>
#include <SFML/Graphics.hpp>
#include "Joint.h"
#include <vector>

class Solver{
public:

    Solver() = default;

    void clear(){
        links.clear();
        points.clear();
        joints.clear();
    }

    VerletPoint::Ptr createPoint(Coords<double>& coords, double mass, bool moving=true){
        VerletPoint::Ptr pt = VerletPoint::create(coords, mass);
        pt->move = moving;
        points.push_back(pt);
        return pt;
    }

    Link::Ptr createLink(VerletPoint::Ptr pt1, VerletPoint::Ptr pt2){
        Link::Ptr link = Link::create(pt1, pt2);
        links.push_back(link);
        return link;
    }

    template<typename... Args>
    Joint::Ptr createJoint(Args&&... args){
        Joint::Ptr joint = Joint::create(args...);
        joints.push_back(joint);
        return joint;
    }

    void applyGravity(){
        for(VerletPoint::Ptr pt : points){
            pt->
        }
    }

    void update(double dt=0.016);

    VerletPoint::Ptr getPointAt(Coords<double>& coords);

    void applyForce(double radius, Coords<double>& coords);

    void render(sf::RenderTarget& target);

    std::vector<VerletPoint::Ptr> getPoints();

private:
    std::vector<VerletPoint::Ptr> points;
    std::vector<Link::Ptr> links;
    std::vector<Joint::Ptr> joints;
};