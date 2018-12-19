#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP

#include <Arduino.h>
#include "motor.hpp"
#include "sensors.hpp"

enum Path {
    longer, shorter
};

enum Edge {
    left, right
};

class Navigation {
private:
    bool finished = false;

    int straight = 0;
    int turning = 0;

    Edge follow_edge = Edge::left;
    long long marker_time;
    int line_crossings = 0;

    Sensors &sensors;
    Motor &left_motor;
    Motor &right_motor;

    void go_straight();

    void go_turn(Motor &inside_motor, Motor &outside_motor);

    void follow_line_edge(bool inside_sensor, Motor &inside_motor, Motor &outside_motor);

    void update_follow_edge();

public:
    Path path_mode = Path::shorter;

    Navigation(Sensors &sensors, Motor &left_motor, Motor &right_motor);

    bool need_immediate_update();

    void navigate();
};

#endif
