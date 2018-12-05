#include "navigation.hpp"

const int FULL_STRAIGHT = 60;
const int FULL_BEND = 30;

int turning = 0;
int straight = 0;

int right_target = FULL_BEND;
int left_target = FULL_BEND;

int i = 0;
unsigned int episode = 0;

int clip(int x, int minimum, int maximum) {
    if (x > maximum)
        x = maximum;
    if (x < minimum)
        x = minimum;
    return x;
}

void navigation(bool slow, Sensors &sensors, Motor &right_motor, Motor &left_motor) {
    if (sensors.center && !sensors.left && !sensors.right) {
        int target = FULL_BEND;
        if (straight > 10)
            target = FULL_STRAIGHT;

        right_target = left_target = target;

        straight++;
        turning = 0;
    } else {
        int target;
        if (turning < 5) target = FULL_BEND / 4;
        if (turning < 10) target = 0;
        if (turning >= 10) target = -(FULL_BEND / 4);

        if (sensors.center && sensors.left) {
            right_target = FULL_BEND;
            left_target = (int) (2 / 3.0 * FULL_BEND);
        } else if (sensors.center && sensors.right) {
            right_target = (int) (2 / 3.0 * FULL_BEND);
            left_target = FULL_BEND;
        } else if (sensors.left) {
            right_target = FULL_BEND / 2;
            left_target = target;
        } else if (sensors.right) {
            right_target = target;
            left_target = FULL_BEND / 2;
        }
        turning++;
        straight = 0;
    }

    if (slow) {
        right_target = clip(right_target, right_target, (int) (2 / 3.0 * FULL_BEND));
        left_target = clip(left_target, left_target, (int) (2 / 3.0 * FULL_BEND));
    }

    right_motor.set_speed(-right_target);
    left_motor.set_speed(left_target);
}
