#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/adi.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"

// Controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::Motor FR (14, pros::MotorGearset::blue);
pros::Motor MR (13, pros::MotorGearset::blue);
pros::Motor BR (12, pros::MotorGearset::blue);
pros::Motor FL (17, pros::MotorGearset::blue);
pros::Motor ML (18, pros::MotorGearset::blue);
pros::Motor BL (19, pros::MotorGearset::blue);

// Left Motor Group
pros::MotorGroup right_motors({12, -13, 14}, pros::MotorGearset::blue);
// Right Motor Group
pros::MotorGroup left_motors({-17, 18, -19}, pros::MotorGearset::blue);

// Subsystem Motors
pros::Motor lowerIntake(-16, pros::MotorGearset::blue);
pros::Motor upperIntake(15, pros::MotorGearset::blue);

//Pneumatics
pros::adi::DigitalOut lift ('F', LOW);
pros::adi::DigitalOut colorSort ('D', LOW);
pros::adi::DigitalOut tongue ('G', LOW);
pros::adi::DigitalOut odomLifter ('C', LOW);
class CustomIMU : public pros::IMU {
public:
  CustomIMU(int port, double scalar)
      : pros::IMU(port), m_port(port), m_scalar(scalar) {}
  virtual double get_rotation() const {
    return pros::c::imu_get_rotation(m_port) * m_scalar;
  }

private:
  const int m_port;
  const double m_scalar;
};

// create custom IMU with a scalar
CustomIMU my_imu(2, 1);

// Vertical Tracking Wheel
pros::Rotation forward_tracker(-3);
// Horizontal Tracking Wheel
pros::Rotation sideways_tracker(4);

pros::adi::Potentiometer auto_selector(5);

// Drivetrain Settings
lemlib::Drivetrain drivetrain(&left_motors, &right_motors, 11.125,
                              lemlib::Omniwheel::NEW_275, 600, 2);

lemlib::TrackingWheel horizontal_tracking_wheel(&sideways_tracker,
                                                lemlib::Omniwheel::NEW_2, -5);
lemlib::TrackingWheel vertical_tracking_wheel(&forward_tracker,
                                              lemlib::Omniwheel::NEW_2, -.5);

lemlib::OdomSensors sensors(&vertical_tracking_wheel, nullptr,
                            &horizontal_tracking_wheel, nullptr, &my_imu);

// Lateral PID Controller
lemlib::ControllerSettings
    lateral_controller(10, 0, 40,
                       3,   // anti windup
                       1,   // small error range, in inches
                       100, // small error range timeout, in milliseconds
                       3,   // large error range, in inches
                       500, // large error range timeout, in milliseconds
                       127   // maximum acceleration (slew)
    );

// Angular PID Controller
lemlib::ControllerSettings
    angular_controller(4.25, 0, 40,
                       3,   // anti windup
                       1,   // small error range, in degrees
                       100, // small error range timeout, in milliseconds
                       3,   // large error range, in degrees
                       500, // large error range timeout, in milliseconds
                       127    // maximum acceleration (slew)
    );

lemlib::Chassis chassis(drivetrain, // Drivetrain Settings
                        lateral_controller, angular_controller,
                        sensors // Odom Sensors
);