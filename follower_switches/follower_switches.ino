#include <Arduino.h>
#include "digital_pins.hpp"
#include "sensors.hpp"
#include "navigation.hpp"

const int MAX_UPDATE = 10;
const int MAX_DELAY = 45;

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
    // motors
    left_motor.attach(LEFT_WHEEL, 500, 2500);
    right_motor.attach(RIGHT_WHEEL, 500, 2500);

    // Sensors
    pinMode(FAR_LEFT, INPUT);
    pinMode(LEFT, INPUT);
    pinMode(CENTER, INPUT);
    pinMode(RIGHT, INPUT);
    pinMode(FAR_RIGHT, INPUT);
    pinMode(LED, OUTPUT);

    // Button
    pinMode(BUTTON, INPUT);
    digitalWrite(BUTTON, HIGH);

    // Serial
    Serial.begin(9600);
    while (sensors.button_pressed()) navigation.path_mode = Path::longer;

    // flash led
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);

    // 1 sec delay
    delay(1000);
}


int started = false;
void loop() {
    // Wait for button press
    if (!started) {
        started = sensors.button_pressed();
        delay(10);
        return;
    }

    // Asynchronous wait for update
    int loop_counter = 0;
    while (!navigation.need_immediate_update() && loop_counter++ < MAX_DELAY) {
        delay(1);
        sensors.update();
    }

    // Navigation
    navigation.navigate();
    sensors.update();
}
