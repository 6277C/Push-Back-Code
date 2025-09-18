#include "lemlib/chassis/chassis.hpp"
#include "main.h"
void auto_0(){
    chassis.moveToPoint(-20, 20, 5000);
    chassis.turnToHeading(135, 5000);
    chassis.moveToPose(20, 20, 90, 5000, {.horizontalDrift = 5, .lead=.5});
}
void auto_1(){}
void auto_2(){}
void auto_3(){}
void auto_4(){}
void auto_5(){}
void auto_6(){}
void auto_7(){}