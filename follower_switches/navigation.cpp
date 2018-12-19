#include "navigation.hpp"
//Todo: Remove this
#include <Arduino.h>
#include "digital_pins.hpp"


const int FULL = 20;
const int MIN_DELAY = 1;

const int TIMEOUT_FIRST = 5;
const int TIMEOUT_SECOND = 20;

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

void Navigation::follow_line_edge(bool inside_sensor, Motor &inside_motor, Motor &outside_motor) {
    if (inside_sensor) go_turn(inside_motor, outside_motor);
    else if (!sensors.center) go_turn(outside_motor, inside_motor);
    else go_straight();
}

void Navigation::update_follow_edge() {
    long current_time = millis() / 1000;

    Serial.println(current_time);
    Serial.println(line_crossings);
    Serial.println("====");

    int time_diff = current_time - marker_time;
    if (time_diff < MIN_DELAY) return;

    // Timeouts for line crossings
    if (line_crossings == 2 && time_diff >= TIMEOUT_FIRST) line_crossings = 0;
    if (line_crossings > 0 && time_diff >= TIMEOUT_SECOND) line_crossings = 0;

    if (line_crossings <= 0) {
        // Right marker
        if (sensors.far_right) {
            if (path_mode == Path::shorter) follow_edge = Edge::right;
            if (path_mode == Path::longer) follow_edge = Edge::left;

            line_crossings = 2;
            marker_time = current_time;
        }

        // Left marker
        if (sensors.far_left) {
            if (path_mode == Path::shorter) follow_edge = Edge::left;
            if (path_mode == Path::longer) follow_edge = Edge::right;

            line_crossings = 2;
            marker_time = current_time;
        }
    } else {
        // Marker when ignoring markers
        if (sensors.far_right_turned_on() && follow_edge == Edge::left ) {
          line_crossings--;
          marker_time = current_time;
        }
        if (sensors.far_left_turned_on() && follow_edge == Edge::right ) {
          line_crossings--;
          marker_time = current_time;
        }
    }
}

Navigation::Navigation(Sensors &sensors, Motor &left_motor, Motor &right_motor) :
        sensors(sensors),
        left_motor(left_motor),
        right_motor(right_motor) {}

bool Navigation::need_immediate_update() {
    // Finish sensors
    bool stop = sensors.far_right && sensors.far_left;
    if (stop) return true;

    // Far sensors off
    if (sensors.far_right_turned_on()) return true;
    if (sensors.far_left_turned_on()) return true;

    // Far sensors on
    if (sensors.far_right_turned_on()) return true;
    if (sensors.far_left_turned_on()) return true;

    // Sensors off
    if (sensors.right_turned_off()) return true;
    if (sensors.left_turned_off()) return true;

    // Sensors on
    if (sensors.right_turned_on()) return true;
    if (sensors.left_turned_on()) return true;

    return false;
}

void Navigation::navigate() {
    if (line_crossings == 0) digitalWrite(LED, HIGH);
    else digitalWrite(LED, LOW);

    // Finished
    if (finished) {
        left_motor.set_speed(0);
        right_motor.set_speed(0);
        return;
    }
    finished = sensors.far_left && sensors.far_right;

    // Follow Edge
    update_follow_edge();
    if (follow_edge == Edge::left) follow_line_edge(sensors.left, left_motor, right_motor);
    if (follow_edge == Edge::right) follow_line_edge(sensors.right, right_motor, left_motor);
}
