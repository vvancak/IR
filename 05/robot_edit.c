#include <Arduino.h>
#include <Servo.h>

// === SERVO CONTROL ===
class motor: public Servo {
public:
    void go(int percentage) {
        writeMicroseconds(1500 + percentage * 2);
    }
};
motor left_motor, right_motor;

// === FUNCTION HEADERS ===
void setup(void);
void loop(void);

bool on_left(void);
bool on_right(void);
bool on_center(void);

// === SENSOR CONTROL ===
const int LEFT_FAR = 3;
const int LEFT = 4;
const int CENTER = 5;
const int RIGHT = 6;
const int RIGHT_FAR = 7;

const int LEFT_WHEEL = 12;
const int RIGHT_WHEEL = 13;

void setup(void) {
    leftMotor.attach(LEFT_WHEEL, 500, 2500);
    rightMotor.attach(RIGHT_WHEEL, 500, 2500);

    pinMode(LEFT_FAR, INPUT);
    pinMode(LEFT, INPUT);
    pinMode(CENTER, INPUT);
    pinMode(RIGHT, INPUT);
    pinMode(RIGHT_FAR, INPUT);

    Serial.begin(115200);
}

// === OUR FUNCTIONS ===
bool on_left(void) {
    return digitalRead(LEFT) == LOW;
}

bool on_right(void) {
    return digitalRead(RIGHT) == LOW;
}

bool on_center(void) {
    return digitalRead(CENTER) == LOW;
}

void go(int left_percent, int right_percent) {
    leftMotor.go(left_percent);
    rightMotor.go(-right_percent);
}


// === CODE VARIABLES HERE ===
const int DELAY = 1;


// === LOOP CODE HERE ===
void loop(void) {
    delay(DELAY);

}


