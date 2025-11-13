#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"
void auto_0(){
    chassis.setPose(0,0,0);
    chassis.moveToPoint(-6, 32, 5000);
    chassis.turnToHeading(90, 5000);
    lowerIntake.move(127);
    //tounge mech down
    chassis.moveToPoint(4, 32, 5000);
    pros::delay(1000);
    chassis.moveToPoint(-6, 33, 5000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 2});
    //tounge mech up
    chassis.turnToHeading(-90, 1000);
    chassis.moveToPoint(-20, 33, 5000);
    //spin top intake
    pros::delay(1000);
    chassis.moveToPoint(0, 32, 5000, {.forwards = false, .minSpeed = 10, .earlyExitRange = 4});
    chassis.moveToPose(-24, 10, -135, 5000, {.lead = 0.2});
    //raise intake
}
void auto_1(){}
void auto_2(){}
void auto_3(){}
void auto_4(){}
void auto_5(){}
void auto_6(){}
void auto_7(){}