#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"

void antiJam(){
    lowerIntake.move(127);
    pros::delay(500);
    while(1){
    if (lowerIntake.get_actual_velocity() < 10 && lowerIntake.get_actual_velocity() > -10){
        lowerIntake.move(-127);
        pros::delay(300);
        lowerIntake.move(127);
    }
    lowerIntake.move(127);
    pros::delay(250);
}
}
void auto_0(){
    chassis.setPose(0,0,0);
    chassis.moveToPoint(-4, 32, 1500);
    chassis.turnToPoint(8, 32, 1000);
    lift.set_value(true);
    // //lowerIntake.move(127);
    pros::Task antiJamTask(antiJam);
    tongue.set_value(true);
    chassis.moveToPoint(9, 32, 1500, {.maxSpeed = 50});
    chassis.moveToPoint(-6, 33, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 2});
    tongue.set_value(false);
    chassis.turnToPoint(-20, 33, 1500);
    chassis.moveToPose(-22, 33, -90, 2000);
    upperIntake.move(127);
    pros::delay(3000);
    upperIntake.brake();
    lift.set_value(false);
    chassis.moveToPoint(0, 33, 2000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 4});
    chassis.moveToPose(-23, 15, -135, 2000, {.lead = 0.2});
    chassis.moveToPoint(-41, -2.5, 5000, {.maxSpeed = 50});
    chassis.waitUntilDone();
    antiJamTask.suspend();
    upperIntake.move(-127);
    lowerIntake.move(-127);
}
void auto_1(){
    chassis.setPose(0,0,0);
    chassis.moveToPoint(4, 32, 1500);
    chassis.turnToPoint(-8, 32, 1000);
    lift.set_value(true);
    // //lowerIntake.move(127);
    pros::Task antiJamTask(antiJam);
    tongue.set_value(true);
    chassis.moveToPoint(-9, 32, 1500, {.maxSpeed = 50});
    chassis.moveToPoint(6, 32, 1000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 2});
    tongue.set_value(false);
    chassis.turnToPoint(20, 32, 1500);
    chassis.moveToPose(22, 32, 90, 2000);
    upperIntake.move(127);
    pros::delay(3000);
    upperIntake.brake();
    lift.set_value(false);
    chassis.moveToPoint(0, 33, 2000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 4});
    chassis.moveToPose(23, 15, 135, 2000, {.lead = 0.2});
    chassis.moveToPoint(41, -2.5, 5000, {.maxSpeed = 50});
    chassis.waitUntilDone();
    antiJamTask.suspend();
    upperIntake.move(127);
    lowerIntake.move(127);
}
void auto_2(){}
void auto_3(){}
void auto_4(){}
void auto_5(){}
void auto_6(){}
void auto_7(){}