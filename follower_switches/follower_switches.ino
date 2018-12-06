#include <Arduino.h>
#include "digital_pins.hpp"
#include "sensors.hpp"
#include "navigation.hpp"

const int MAX_UPDATE = 10;
const int MAX_DELAY = 20;

// Function Headers
void setup(void);

void loop(void);

// Motor controllers
Motor right_motor(MAX_UPDATE, true);
Motor left_motor(MAX_UPDATE, false);

// Main Control Classes
Sensors sensors;
Navigation navigation(sensors, left_motor, right_motor);

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

void loop() {
int loop_counter = 0;
    while (!navigation.need_immediate_update() && loop_counter++ < MAX_DELAY) {
        delay(1);
        sensors.update();
    }

    navigation.navigate();
    sensors.update();
}
