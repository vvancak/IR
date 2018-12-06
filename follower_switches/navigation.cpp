#include "navigation.hpp"
//Todo: Remove this
#include <Arduino.h>
#include "digital_pins.hpp"


const int FULL = 30;

void Navigation::update_state() {
    switch (state) {

        // Line following
        case State::nominal:
            if (sensors.far_left && sensors.far_right) state = State::finished;
            else if (sensors.far_right) {
                if (sensors.button_mode == Mode::shortest) sw_state = right;
                if (sensors.button_mode == Mode::longest) sw_state = left;
                digitalWrite(LED, HIGH);
                state = State::before_switch;
            } else if (sensors.far_left) {
                if (sensors.button_mode == Mode::shortest) sw_state = left;
                if (sensors.button_mode == Mode::longest) sw_state = right;
                digitalWrite(LED, LOW);
                state = State::before_switch;
            }
            break;

            // Before Switch
        case State::before_switch:
            if (sensors.center) {
                if (sw_state == right && sensors.left) state = State::on_switch;
                if (sw_state == left && sensors.right) state = State::on_switch;
            }
            break;

            // Switch
        case State::on_switch:
            if (sensors.center) {
                if (sw_state == right && !sensors.left) state = State::between_switch;
                if (sw_state == left && !sensors.right) state = State::between_switch;
            }
            break;

            // Follow the line between switch and joint back into one track
        case State::between_switch:
            if (sensors.center) {
                if (sw_state == right && sensors.left) state = State::on_joint;
                if (sw_state == left && sensors.right) state = State::on_joint;
            }
            break;

            // Join back to the track
        case State::on_joint:
            if (sensors.center) {
                if (sw_state == right && !sensors.left) state = State::nominal;
                if (sw_state == left && !sensors.right) state = State::nominal;
            }
            break;
    }
}

void Navigation::go_straight() {
    left_motor.set_speed(FULL);
    right_motor.set_speed(FULL);

    ++straight;
    turning = 0;
}

void Navigation::go_turn(Motor &inside_motor, Motor &outside_motor) {
    inside_motor.set_speed(-FULL / 3);
    outside_motor.set_speed(FULL);

    ++turning;
    straight = 0;
}

void Navigation::follow_line() {
    if (sensors.left) go_turn(left_motor, right_motor);
    else if (sensors.right) go_turn(right_motor, left_motor);
    else if (sensors.center) go_straight();
}

void Navigation::follow_line_edge(bool inside_sensor, Motor &inside_motor, Motor &outside_motor) {
    bool center_sensor = sensors.center;

    if (inside_sensor) go_turn(inside_motor, outside_motor);
    else if (!center_sensor) go_turn(outside_motor, inside_motor);
    else go_straight();
}

Navigation::Navigation(Sensors &sensors, Motor &left_motor, Motor &right_motor) :
        sensors(sensors),
        left_motor(left_motor),
        right_motor(right_motor) {}

bool Navigation::need_immediate_update() {
    bool sign = sensors.far_right || sensors.far_left;
    if (sign) return true;

    bool turning = sensors.left || sensors.right;
    if (turning && straight > 0) return true;

    bool straight = !sensors.left && !sensors.right;
    if (straight && turning > 0) return true;

    return false;
}

void Navigation::navigate() {
    update_state();

    // Finished => Stop
    if (state == State::finished) {
        left_motor.set_speed(0);
        right_motor.set_speed(0);
    }

        // Nominal => Line Follower
    else if (state == State::nominal) {
        follow_line();
    }

        // Anything else - Follow line edge according to the button
    else {
        if (sw_state == Switch::left) follow_line_edge(sensors.left, left_motor, right_motor);
        if (sw_state == Switch::right) follow_line_edge(sensors.right, right_motor, left_motor);
    }
}
