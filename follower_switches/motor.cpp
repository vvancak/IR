#include "motor.hpp"

Motor::Motor(int max_update, bool reversed) {
    _reversed = reversed;
    _max_update = max_update;
    _current_speed = 0;
}

void Motor::set_speed(int percentage) {
    int diff = percentage - _current_speed;
    if (diff > _max_update) diff = _max_update;
    if (diff < -_max_update) diff = -_max_update;

    _current_speed += diff;
    go(_current_speed);
}

void Motor::go(int percentage) {
    if (_reversed) percentage = -percentage;
    writeMicroseconds(1500 + percentage * 2);
}
