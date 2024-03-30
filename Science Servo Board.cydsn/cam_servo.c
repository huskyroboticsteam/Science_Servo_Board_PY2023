/* File:         camservo.c
 * Authors:      Hayden Rundle
 * Organization: Husky Robotics Team
 * 
 * This file includes fuction implementations for the servos on the 
 * Science Board including continuous servos
 */
#include <stdint.h>
#include "CANLibrary.h"
#include "CANScience.h"
#include "PCA9685.h"
#include "project.h"
#include "cam_servo.h"

#define PWM_PERIOD 20 //pwm period ms

//constants for actual LED pin number on board
//#define CUP_LIDS_SERVO 1
//#define CAMERA_SERVO 5
//#define DRILL_ARM 7


void set_servo_position(uint8_t servo, uint8_t degrees){
    // float32 min;
    // float32 range;
    // float32 offset;
    float32 pwmDuty;
    int found = 1;
    if(degrees > 179) degrees = 179; //may be different for each case
	if(degrees < 1) degrees = 1;
	if (servo < 0) {// || servo > 4){
		return;
	}
    switch(servo) {
        //case numbers were arbitrarily defined by software
//        case DRILL_COVER_ID : // Drill Cover Servo
//            servo = DRILL_COVER_SERVO_LED_NUM;
//            break;
//        case 4 : //Tower Pro SG90 //CAMERA TILT
//            servo = CAMERA_SERVO;
//            break;
//        case 5: //Tower Pro SG90 //DRILL ARM
//            servo = DRILL_ARM;
//            break;
        default: 
            found = 0;
//            ERR_LED_Write(0);
            //CyDelay(500);
//            ERR_LED_Write(1);
            break;
    }
    //offset = ((float)degrees / 180) * range; 
    if (found) {
        pwmDuty = (degrees/((float32)180))*5 + 5;
   	    setPWMFromDutyCycle(servo, pwmDuty);
    }
}
/* [] END OF FILE */