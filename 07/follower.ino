#include <Arduino.h>
#include "digital_pins.hpp"
#include "sensors.hpp"
#include "navigation.hpp"

// Function Headers
void setup(void);

void navigation(bool slow);

void loop(void);

// CONSTANTS
const int MAX_UPDATE = 10;

Motor right_motor(MAX_UPDATE);
Motor left_motor(MAX_UPDATE);

Sensors sensors;
State last_state = State::nominal;

void setup() {
    left_motor.attach(LEFT_WHEEL, 500, 2500);
    right_motor.attach(RIGHT_WHEEL, 500, 2500);

    pinMode(FAR_LEFT, INPUT);
    pinMode(LEFT, INPUT);
    pinMode(CENTER, INPUT);
    pinMode(RIGHT, INPUT);
    pinMode(FAR_RIGHT, INPUT);
    pinMode(LED, OUTPUT);
    pinMode(BUTTON, INPUT);

    Serial.begin(9600);
}

void loop() {
    auto state = sensors.update();

    if(state == finished)
    {
        right_motor.set_speed(0);
        left_motor.set_speed(0);
        
        return;
    }

    if (state != last_state)
    {
        navigation(true, sensors, right_motor, left_motor);
        last_state = state;
    } else
        navigation(false, sensors, right_motor, left_motor);

    delay(50);
}
