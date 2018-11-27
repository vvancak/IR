#include <Arduino.h> 
#include <Servo.h>
class motor: public Servo {
  public:

    void go(int percentage) {
      writeMicroseconds(1500 + percentage * 2);
    }

};
motor leftMotor, rightMotor;

void setup(void);
void loop(void);

void print_sensors(void);

bool on_left_sensor(void);
bool on_right_sensor(void);
bool on_center_sensor(void);
bool finished(void);

const int LEFT_FAR = 3;
const int LEFT = 4;
const int CENTER = 5;
const int RIGHT = 6;
const int RIGHT_FAR = 7;

const int LEFT_WHEEL = 12;
const int RIGHT_WHEEL = 13;

const int FULL = 50;

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

double right_speed = 50;
double left_speed = 50;

void loop(void) {
  if (finished()) {
    leftMotor.go(0);
    rightMotor.go(0);
    return;
  }

  if (on_center_sensor()) {
    left_speed += 1;
    right_speed += 1;

    if (left_speed >= FULL) left_speed = FULL;
    if (right_speed >= FULL) right_speed = FULL;

    if (left_speed <= -FULL) left_speed = -FULL;
    if (right_speed <= -FULL) right_speed = -FULL;
  }

  if (on_left_sensor()) {
    left_speed = 0;
  }

  if (on_right_sensor()) {
    right_speed = 0;
  }

  leftMotor.go((int)(left_speed / 2.5));
  rightMotor.go((int)(-right_speed / 2.5));

}

bool on_left_sensor(void) {
  return digitalRead(LEFT) == LOW;
}

bool on_right_sensor(void) {
  return digitalRead(RIGHT) == LOW;
}

bool on_center_sensor(void) {
  return digitalRead(CENTER) == LOW;
}

bool finished(void) {
  return digitalRead(LEFT_FAR) == LOW &&
    digitalRead(RIGHT_FAR) == LOW;
}

void print_sensors() {
  Serial.print("Left:\n");
  Serial.print(digitalRead(LEFT));
  Serial.print("\nRight:\n");
  Serial.print(digitalRead(RIGHT));
  Serial.print("\nCenter:\n");
  Serial.print(digitalRead(CENTER));
  Serial.print("\nSpeeds:\n");
}
