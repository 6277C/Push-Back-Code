#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"

// Controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// Left Motor Group
pros::MotorGroup left_motors({-1, -2, -3}, pros::MotorGearset::blue);
// Right Motor Group
pros::MotorGroup right_motors({4, 5, 6}, pros::MotorGearset::blue);

// Subsystem Motors
pros::Motor intake(10, pros::MotorGearset::green);

// IMU (Inertial Sensor)
pros::IMU imu(10);

// Vertical Tracking Wheel
pros::Rotation forward_tracker(7);
// Horizontal Tracking Wheel
pros::Rotation sideways_tracker(8);

pros::adi::Potentiometer auto_selector(1);

// Drivetrain Settings
lemlib::Drivetrain drivetrain(&left_motors, &right_motors, 14,
                              lemlib::Omniwheel::NEW_275, 600, 2);

lemlib::TrackingWheel horizontal_tracking_wheel(&sideways_tracker,
                                                lemlib::Omniwheel::NEW_2, -1);
lemlib::TrackingWheel vertical_tracking_wheel(&forward_tracker,
                                              lemlib::Omniwheel::NEW_2, -1);

lemlib::OdomSensors sensors(&vertical_tracking_wheel, nullptr,
                            &horizontal_tracking_wheel, nullptr, &imu);

// Lateral PID Controller
lemlib::ControllerSettings
    lateral_controller(10, 0, 3,
                       3,   // anti windup
                       1,   // small error range, in inches
                       100, // small error range timeout, in milliseconds
                       3,   // large error range, in inches
                       500, // large error range timeout, in milliseconds
                       20   // maximum acceleration (slew)
    );

// Angular PID Controller
lemlib::ControllerSettings
    angular_controller(2, 0, 10,
                       3,   // anti windup
                       1,   // small error range, in degrees
                       100, // small error range timeout, in milliseconds
                       3,   // large error range, in degrees
                       500, // large error range timeout, in milliseconds
                       0    // maximum acceleration (slew)
    );

lemlib::Chassis chassis(drivetrain, // Drivetrain Settings
                        lateral_controller, angular_controller,
                        sensors // Odom Sensors
);