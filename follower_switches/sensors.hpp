#ifndef SENSORS_HPP
#define SENSORS_HPP

#include <Arduino.h>
#include "digital_pins.hpp"

class Sensors {
public:
    bool far_left = 0;
    bool left = 0;
    bool center = 0;
    bool right = 0;
    bool far_right = 0;

    bool prev_far_left = 0;
    bool prev_left = 0;
    bool prev_right = 0;
    bool prev_far_right = 0;

private:
    bool read_right();

    bool read_left();

    bool read_center();

    bool read_far_right();

    bool read_far_left();

public:
    void update();

    bool button_pressed();

    bool left_turned_on();

    bool left_turned_off();

    bool right_turned_on();

    bool right_turned_off();

    bool far_left_turned_on();

    bool far_left_turned_off();

    bool far_right_turned_on();

    bool far_right_turned_off();
};

#endif
