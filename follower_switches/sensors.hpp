#ifndef SENSORS_HPP
#define SENSORS_HPP

#include <Arduino.h>
#include "digital_pins.hpp"

enum Mode {
    shortest, longest
};

class Sensors {
public:
    bool far_left = 0;
    bool left = 0;
    bool center = 0;
    bool right = 0;
    bool far_right = 0;
    Mode button_mode = Mode::shortest;

private:
    bool read_right();

    bool read_left();

    bool read_center();

    bool read_far_right();

    bool read_far_left();

    bool read_button();

public:
    void update();
};

#endif
