#include "sensors.hpp"

bool Sensors::read_right() {
    return digitalRead(RIGHT) == LOW;
}

bool Sensors::read_left() {
    return digitalRead(LEFT) == LOW;
}

bool Sensors::read_center() {
    return digitalRead(CENTER) == LOW;
}

bool Sensors::read_far_right() {
    return digitalRead(FAR_RIGHT) == LOW;
}

bool Sensors::read_far_left() {
    return digitalRead(FAR_LEFT) == LOW;
}

void Sensors::update() {
    // Update Switch-Sensors
    prev_far_left = far_left;
    prev_far_right = far_right;

    far_left = read_far_left();
    far_right = read_far_right();

    // Update line following sensors only if one of them sees something
    bool new_right = read_right();
    bool new_left = read_left();
    bool new_center = read_center();

    if (new_right || new_left || new_center) {
        prev_left = left;
        prev_right = right;

        left = new_left;
        right = new_right;
        center = new_center;
    }
}

bool Sensors::button_pressed() {
    return digitalRead(BUTTON) == LOW;
}

bool Sensors::left_turned_on() {
    return left && !prev_left;
}

bool Sensors::left_turned_off() {
    return !left && prev_left;
}

bool Sensors::right_turned_on() {
    return right && !prev_right;
}

bool Sensors::right_turned_off() {
    return !right && prev_right;
}

bool Sensors::far_left_turned_on() {
    return far_left && !prev_far_left;
}

bool Sensors::far_left_turned_off() {
    return !far_left && prev_far_left;
}

bool Sensors::far_right_turned_on() {
    return far_right && !prev_far_right;
}

bool Sensors::far_right_turned_off() {
    return !far_right && prev_far_right;
}
