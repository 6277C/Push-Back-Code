#include "pros/misc.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/adi.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"

#ifndef robot_config
#define robot_config
extern pros::Controller controller;
extern pros::MotorGroup left_motors;
extern pros::MotorGroup right_motors;
extern pros::Motor lowerIntake;
extern pros::Motor upperIntake;
extern pros::adi::DigitalOut lift;
extern pros::adi::DigitalOut colorSort;
extern pros::IMU imu;
extern pros::Rotation forward_tracker;
extern pros::Rotation sideways_tracker;
extern pros::adi::Potentiometer auto_selector;
extern lemlib::Drivetrain drivetrain;
extern lemlib::TrackingWheel horizontal_tracking_wheel;
extern lemlib::TrackingWheel vertical_tracking_wheel;
extern lemlib::OdomSensors sensors;
extern lemlib::ControllerSettings lateral_controller;
extern lemlib::ControllerSettings angular_controller;
extern lemlib::Chassis chassis;
#endif 