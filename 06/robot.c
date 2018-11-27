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
void go(int, int );
int get_diff(int , int , const int);
void update_sensors(void);


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
bool on_right()
{
    return camera_on_line(right_camera);
}

bool on_left()
{
    return camera_on_line(left_camera);
}

bool on_center()
{
    return camera_on_line(center_camera);
}

void go(int left_percent, int right_percent)
{
    motor_go_percent(left_motor, left_percent);
    motor_go_percent(right_motor, right_percent);
}

int get_diff(int current_speed, int target_speed, const int CLIP)
{
    int diff = target_speed - current_speed;
    int abs_diff = abs(diff);

    if(abs_diff > CLIP)
    {
        diff = CLIP * (diff / abs_diff);
    }

    return diff;
}

// === OUR STRUCTURES ===
struct Sensors
{
    bool on_left;
    bool on_right;
    bool on_center;
} s;

void update_sensors(void)
{
    bool right = on_right();
    bool left = on_left();
    bool center = on_center();

    // Update only if some signal is found
    if(right || left || center)
    {
        s.on_left = left;
        s.on_right = right;
        s.on_center = center;
    }
}


// === CODE VARIABLES HERE ===
const int DELAY = 50;

const int CLIP = 10;
const int FULL = 30;
const int FULL_STRAIGHT = 50;

int right_target = FULL;
int left_target = FULL;

int right_speed = 0;
int left_speed = 0;

int turning = 0;
int straight = 0;

// === LOOP CODE HERE ===
void loop(void) {
    delay(DELAY);
    right_target = right_speed;
    left_target = left_speed;

    update_sensors();
    int turning_target_speed;

    if(s.on_left || s.on_right)
    {
        ++turning;
        straight = 0;

        if(turning < 5)
            turning_target_speed = FULL / 2;
        if(turning < 10)
            turning_target_speed = 0;
        if(turning >= 10)
            turning_target_speed = -(FULL / 2);
    }

    // slight right
    if(s.on_center && s.on_left)
    {
        right_target = FULL;
        left_target = (int)(2 / 3.0 * FULL);

    }
    // slight left
    else if(s.on_center && s.on_right)
    {
        right_target = (int)(2 / 3.0 * FULL);
        left_target = FULL;

    }

    // go straight
    else if(s.on_center)
    {
        turning = 0;
        ++straight;

        int straight_target_speed = FULL;
        if(straight > 10)
            straight_target_speed = FULL_STRAIGHT;

        right_target = straight_target_speed;
        left_target = straight_target_speed;
    }
    // hard left
    else if(s.on_left)
    {
        right_target = FULL / 2;
        left_target = turning_target_speed;

    }
    // hard right
    else if(s.on_right)
    {
        right_target = turning_target_speed;
        left_target = FULL / 2;
    }

    right_speed += get_diff(right_speed, right_target, CLIP);
    left_speed += get_diff(left_speed, left_target, CLIP);

    go(left_speed, right_speed);
}