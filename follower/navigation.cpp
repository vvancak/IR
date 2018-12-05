#include "navigation.hpp"

const int FULL_STRAIGHT = 70;
const int FULL_BEND = 50;

int turning = 0;
int straight = 0;

int right_target = FULL_BEND;
int left_target = FULL_BEND;

int i = 0;
unsigned int episode = 0;

bool need_immediate_update(Sensors &sensors) {
    if (sensors.far_left || sensors.far_right) return true;

    bool in_turn = sensors.left || sensors.right;

    if (!in_turn && turning) return true;
    if (in_turn && straight) return true;

    return false;
}


void navigation(Sensors &sensors, Motor &right_motor, Motor &left_motor) {
    if (sensors.center && !sensors.left && !sensors.right) {
        int target = FULL_BEND;
        if (straight > 10)
            target = FULL_STRAIGHT;

        right_target = left_target = target;

        straight++;
        turning = 0;
    } else {
        int target;
        if (turning < 5) target = FULL_BEND / 3;
        if (turning < 10) target = 0;
        if (turning >= 10) target = -(FULL_BEND / 3);

        if (sensors.center && sensors.left) {
            right_target = FULL_STRAIGHT;
            left_target = FULL_BEND;
        } else if (sensors.center && sensors.right) {
            right_target = FULL_BEND;
            left_target = FULL_STRAIGHT;
        } else if (sensors.left) {
            right_target = FULL_BEND;
            left_target = target;
        } else if (sensors.right) {
            right_target = target;
            left_target = FULL_BEND;
        }
        turning++;
        straight = 0;
    }

    right_motor.set_speed(-right_target);
    left_motor.set_speed(left_target);
}