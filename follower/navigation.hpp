#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP

#include <Arduino.h>
#include "motor.hpp"
#include "sensors.hpp"

bool need_immediate_update(Sensors &sensors);

void navigation(Sensors &sensors, Motor &right_motor, Motor &left_motor);

#endif
