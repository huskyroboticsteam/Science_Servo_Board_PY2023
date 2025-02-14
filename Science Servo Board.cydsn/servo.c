/* File:         sensor.c
 * Authors:      
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
#include "servo.h"

int16 current_servo_values[16] = {};

// sets the PCA duty cycle for positional servos 1-10
// degrees is from 0 to 180
int set_servo_position(int servo, int degrees){
    float32 duty;
    if(degrees > 180) degrees = 180;
	if(degrees < 0) degrees = 0;
	if (servo < 0 || servo > 9){ 
        // UART("Invaid servo"); don't know how to print
        return 1;
    }
    current_servo_values[servo] = degrees;
    
    duty = (degrees/180.0)*5 + 5;
   	setPWMFromDutyCycle(servo, duty);
    return 0;
}


/* [] END OF FILE */
