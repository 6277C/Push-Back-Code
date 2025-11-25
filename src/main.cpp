#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "liblvgl/llemu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include <cmath>
#include <cstdio>
#include <stdio.h>
#include <string>

using namespace pros;

// GLOBALS
bool logging = true;
bool ss1 = false;
bool liftState = false;
bool tongueState = false;
bool colorSortState = false;
int liftcount = 0;
int tonguecount = 0;

std::string getNewLogFilename() {
  int index = 0;

  while (true) {
    std::string path = "/usd/log" + std::to_string(index) + ".csv";

    // Try opening in read mode to test if it exists
    FILE *f = fopen(path.c_str(), "r");
    if (!f) {
      // File does NOT exist → use this name
      return path;
    }

    // File exists → close and increment
    fclose(f);
    index++;
  }
}
void dataLogging() {
  std::string filename = getNewLogFilename();
  FILE *logFile = fopen(filename.c_str(), "w");
  // FILE* logFile = fopen("/usd/log.csv", "w");
  fputs("Time, Battery Pct., FRTemp, MRTemp, BRTemp, FLTemp, MLTemp, BLTemp, "
        "LITemp, UITemp, LiftFirings, MLFirings\n",
        logFile);
  while (logging == true) {
    std::string FRtemp = std::to_string(std::round(FR.get_temperature()));
    std::string MRtemp = std::to_string(std::round(MR.get_temperature()));
    std::string BRtemp = std::to_string(std::round(BR.get_temperature()));
    std::string FLtemp = std::to_string(std::round(FL.get_temperature()));
    std::string MLtemp = std::to_string(std::round(ML.get_temperature()));
    std::string BLtemp = std::to_string(std::round(BL.get_temperature()));
    std::string LItemp = std::to_string(std::round(lowerIntake.get_temperature()));
    std::string UItemp = std::to_string(std::round(upperIntake.get_temperature()));
    std::string battery = std::to_string(battery::get_capacity());
    std::string time = std::to_string(std::round(pros::millis() / 1000));
    std::string liftcountstr = std::to_string(liftcount);
    std::string tonguecountstr = std::to_string(tonguecount);
    fprintf(logFile, "%s,", time.c_str());
    fprintf(logFile, "%s,", battery.c_str());
    fprintf(logFile, "%s,", FRtemp.c_str());
    fprintf(logFile, "%s,", MRtemp.c_str());
    fprintf(logFile, "%s,", BRtemp.c_str());
    fprintf(logFile, "%s,", FLtemp.c_str());
    fprintf(logFile, "%s,", MLtemp.c_str());
    fprintf(logFile, "%s,", BLtemp.c_str());
    fprintf(logFile, "%s,", LItemp.c_str());
    fprintf(logFile, "%s,", UItemp.c_str());
    fprintf(logFile, "%s,", liftcountstr.c_str());
    fprintf(logFile, "%s,", tonguecountstr.c_str());
    fprintf(logFile, "\n");
    delay(1000);
  }
  fclose(logFile);
}
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  lcd::initialize();   // initialize brain screen
  chassis.calibrate(); // calibrate sensors
  if (usd::is_installed()){  // check if sd card is installed and run logger
  Task datalogger(dataLogging);
  }
  // print position to brain screen
  Task screen_task([&]() {
    while (true) {
      // print robot location to the brain screen
      lcd::print(0, "X: %f", chassis.getPose().x);         // x
      lcd::print(1, "Y: %f", chassis.getPose().y);         // y
      lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
      delay(20);
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
void auto_toggle() {}
void competition_initialize() {
  // pros::lcd::register_btn0_cb(auto_toggle);
}

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
void autonomous() {
  // int autoSelection = (auto_selector.get_value() /
  //                     128   //divide the pot max value (1024) by the number
  //                     of autos to get this value
  // );
  int autoSelection = 1;
  switch (autoSelection) {
  case 0:
    auto_0();
    break;
  case 1:
    auto_1();
    break;
  case 2:
    auto_2();
    break;
  case 3:
    auto_3();
    break;
  case 4:
    auto_4();
    break;
  case 5:
    auto_5();
    break;
  case 6:
    auto_6();
    break;
  case 7:
    auto_7();
    break;
  }
}

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

void opcontrol() {
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
  lowerIntake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  upperIntake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  while (true) {
    // subsystem toggle code
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
      ss1 = !ss1;
    }

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
      lowerIntake.move(-127);
    } else {
      if (ss1) {
        lowerIntake.move(127);
      } else {
        lowerIntake.brake();
      }
    }

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      upperIntake.move(127);
    } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      upperIntake.move(-127);
    } else {
      upperIntake.brake();
    }

    // Pneumatics
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
      liftState = !liftState;
      lift.set_value(liftState);
      if (liftState) {
        liftcount++;
      }
    }

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
      tongueState = !tongueState;
      tongue.set_value(tongueState);
      if (tongueState) {
        tonguecount++;
      }
    }

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
      colorSortState = !colorSortState;
      colorSort.set_value(colorSortState);
    }

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)){
      odomLifter.set_value(HIGH);
    }

    //Logging Exit
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
      logging = false;
      controller.rumble(".-.");
    }

    // get left y and right y positions
    int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

    // control the robot
    chassis.tank(leftY, rightY);

    // delay to save resources
    delay(25);
  }
}