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

bool Sensors::read_button() {
    return digitalRead(BUTTON) == HIGH;
}

void Sensors::update() {

    // Update Button Mode
    if (read_button()) {
        if (button_mode == Mode::shortest) button_mode = Mode::longest;
        else button_mode = Mode::shortest;
    }

    // Update Switch-Sensors
    far_left = read_far_left();
    far_right = read_far_right();

    // Update line following sensors only if one of them sees something
    bool new_right = read_right();
    bool new_left = read_left();
    bool new_center = read_center();
    if (new_right || new_left || new_center) {
        left = new_left;
        right = new_right;
        center = new_center;
    }
}
