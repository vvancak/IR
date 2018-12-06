#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP

#include <Arduino.h>
#include "motor.hpp"
#include "sensors.hpp"

enum State {
    finished, nominal, before_switch, on_switch, between_switch, on_joint
};

enum Switch {
    left, right
};

class Navigation {
private:
    int straight = 0;
    int turning = 0;

    State state = State::nominal;
    Switch sw_state = Switch::left;

    Sensors &sensors;
    Motor &left_motor;
    Motor &right_motor;

    void update_state();

    void go_straight();

    void go_turn(Motor &inside_motor, Motor &outside_motor);

    void follow_line();

    void follow_line_edge(bool inside_sensor, Motor &inside_motor, Motor &outside_motor);

public:
    Navigation(Sensors &sensors, Motor &left_motor, Motor &right_motor);

    bool need_immediate_update();

    void navigate();
};

#endif
