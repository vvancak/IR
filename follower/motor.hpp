#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <Servo.h>
#include <Arduino.h>

class Motor : public Servo {
public:
    Motor(int max_update);

    void set_speed(int percentage);

private:
    int _max_update;
    int _current_speed;

    void go(int percentage);
};

#endif
