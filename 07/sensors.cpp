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
    if (digitalRead(BUTTON) == HIGH) button = !button;
}

State Sensors::update(void) {
    bool new_right = read_right();
    bool new_left = read_left();
    bool new_center = read_center();
    bool new_far_left = read_far_left();
    bool new_far_right = read_far_right();

    // Update only if some signal is found
    if (new_right || new_left || new_center) {
        left = new_left;
        right = new_right;
        center = new_center;
        far_left = new_far_left;
        far_right = new_far_right;

        //if mutiple sensors active, use the rightest only and switch to on_right_turn
        if (state == State::right_sign) {
            left = false;
            far_left = false;

            if (center_off > 0) {
                center = false;
                center_off--;
            }

            if (new_left)
                real_left++;
            if (new_right) {
                center = false;
                center_off = 5;
            }

            if (!new_left && real_left > 0)
                left = new_left;

            if (new_far_left)
                real_far_left++;
            if (!new_far_left && real_far_left > 0) {
                far_left = new_far_left;
                state = State::nominal;
                real_far_left = 0;
                real_left = 0;
                center_off = 0;
            }

        } else if (state == State::left_sign) {
            right = false;
            far_right = false;

            if (center_off > 0) {
                center = false;
                center_off--;
            }

            if (new_right)
                real_right++;
            if (new_left) {
                center = false;
                center_off = 5;
            }

            //start using sensors after line cross
            if (!new_right && real_right > 0)
                right = new_right;

            if (new_far_right)
                real_far_right++;
            if (!new_far_right && real_far_right > 0) {
                far_right = new_far_right;
                state = State::nominal;
                real_far_right = 0;
                real_right = 0;
                center_off = 0;
            }
        }
    }
    if (far_left && far_right) {
        state = State::finished;
    } else if (far_left && state != State::right_sign) {
        if (mode == Mode::away)
            state == State::right_sign;
        else
            state = State::left_sign;
    } else if (far_right && state != State::left_sign) {
        if (mode == Mode::away)
            state = State::left_sign;
        else
            state = State::right_sign;
    }

    return state;
}
