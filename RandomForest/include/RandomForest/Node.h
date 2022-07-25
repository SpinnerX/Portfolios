#include <iostream>
#include "Node_Conf.h"

class Node{
    using Ptr = std::shared_ptr<Node>;
public:
    Node();
    Node(Node_Conf& other);

    Vec2<double> position() const { return configuration.pos(); }
    Vec2<double> growth() const { return configuration.growth(); }
    double length() const { return configuration.len(); }
    double width() const { return configuration.width(); }
    double level() const { return configuration.lvl(); }
    double index() const { return configuration.index(); }
    bool isDone() const { return configuration.isDone(); }

    Vec2<double> getEnd() {
        return configuration.getEnd();
    }

    template<typename... Args>
    static Ptr create(Args&& ...args) { return std::make_shared<Node>(args...); }

    void operator=(const Node_Conf& other){
        configuration = other;
    }

private:
    Node_Conf configuration;
};