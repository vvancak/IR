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
    digitalWrite(BUTTON, HIGH);

    Serial.begin(9600);
}

auto state = sensors.update();

void loop() {
    if (state == finished) {
        right_motor.set_speed(0);
        left_motor.set_speed(0);
        return;
    }

    state = sensors.update();
    int loop_counter = 0;

    while (!need_immediate_update(sensors) && ++loop_counter < 50) {
        delay(1);
        state = sensors.update();
    }

    navigation(sensors, right_motor, left_motor);
    last_state = state;
}
