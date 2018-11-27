#include <webots/robot.h>
#include <webots/distance_sensor.h>
#include <webots/light_sensor.h>
#include <webots/led.h>
#include <webots/camera.h>
#include <webots/motor.h>

#include <stdlib.h>
#include <stdio.h>

// === SENSORS ===
WbDeviceTag left_camera;
WbDeviceTag center_camera;
WbDeviceTag right_camera;

WbDeviceTag left_motor;
WbDeviceTag right_motor;

// === CONSTANTS ===
const int SENSOR_REFRESH_STEP = 1;    // delay between camera refresh
const int MAX_SPEED = 20;    // robot limits in simulator

// === SENSOR CONTROL ===
bool camera_on_line(WbDeviceTag camera)
{
    int width = wb_camera_get_width(camera);
    const unsigned char* image = wb_camera_get_image(camera);
    return wb_camera_image_get_gray(image, width, 0, 0) < 127;
}

void motor_go_percent(WbDeviceTag motor, int percent)
{
    double percent_d = percent / 100.0;
    double speed = percent_d * MAX_SPEED;
    wb_motor_set_velocity(motor, speed);
}

void sensor_init()
{
    // cameras
    left_camera = wb_robot_get_device("left_camera");
    center_camera = wb_robot_get_device("center_camera");
    right_camera = wb_robot_get_device("right_camera");

    wb_camera_enable(left_camera, SENSOR_REFRESH_STEP);
    wb_camera_enable(center_camera, SENSOR_REFRESH_STEP);
    wb_camera_enable(right_camera, SENSOR_REFRESH_STEP);

    // motors
    left_motor = wb_robot_get_device("left wheel motor");
    right_motor = wb_robot_get_device("right wheel motor");

    wb_motor_set_position(left_motor, INFINITY);
    wb_motor_set_position(right_motor, INFINITY);
    wb_motor_set_velocity(left_motor, 0.0);
    wb_motor_set_velocity(right_motor, 0.0);
}


// === OUR FUNCTIONS HERE ===
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

// === MAIN ===
int main()
{
    wb_robot_init();
    sensor_init();

    // === CODE VARIABLES HERE ===
    const int DELAY = 50;

    const int CLIP = 10;
    const int FULL = 30;
    const int FULL_STRAIGHT = 660;

    int right_target = FULL;
    int left_target = FULL;

    int right_speed = 0;
    int left_speed = 0;

    int turning = 0;
    int straight = 0;

    while(wb_robot_step(DELAY) != -1)
    {
        // === LOOP CODE HERE ===
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

    wb_robot_cleanup();
    return 0;
}









