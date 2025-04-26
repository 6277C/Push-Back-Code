#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/misc.h"
#include "pros/motor_group.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"

//Controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

//Left Motor Group
pros::MotorGroup left_motors({-1, -2, -3}, pros::MotorGearset::blue);
//Right Motor Group
pros::MotorGroup right_motors({4, 5, 6}, pros::MotorGearset::blue);

//Subsystem Motors
pros::Motor intake(10, pros::MotorGearset::green);

//Drivetrain Settings
lemlib::Drivetrain drivetrain(&left_motors,
                              &right_motors,
                              14,
                              lemlib::Omniwheel::NEW_275,
                              600,
                              2
);

//IMU (Inertial Sensor)
pros::IMU imu(10);

//Vertical Tracking Wheel
pros::Rotation forward_tracker(7);
//Horizontal Tracking Wheel
pros::Rotation sideways_tracker(8);
lemlib::TrackingWheel horizontal_tracking_wheel(&sideways_tracker, lemlib::Omniwheel::NEW_2, -1);
lemlib::TrackingWheel vertical_tracking_wheel(&forward_tracker, lemlib::Omniwheel::NEW_2, -1);

lemlib::OdomSensors sensors(&vertical_tracking_wheel,
                            nullptr,
                            &horizontal_tracking_wheel,
                            nullptr,
                            &imu
);

//Lateral PID Controller
lemlib::ControllerSettings lateral_controller(10,
                                              0,
                                              3,
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

//Angular PID Controller
lemlib::ControllerSettings angular_controller(2,
                                              0,
                                              10,
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

lemlib::Chassis chassis(drivetrain, //Drivetrain Settings
						lateral_controller,
						angular_controller,
						sensors //Odom Sensors
);


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors
    // print position to brain screen
    pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            pros::delay(20);
        }
    });
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
bool ss1 = false;
void opcontrol() {
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	while (true) {
        // subsystem toggle code
        if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
            ss1 = !ss1;
        }

        if(ss1){
            intake.move(127);
        } else{
            intake.brake();
        }

        // get left y and right y positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

        // move the robot
        chassis.tank(leftY, rightY);

        // delay to save resources
        pros::delay(25);
    }
}