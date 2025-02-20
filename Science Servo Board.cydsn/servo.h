/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <stdint.h>
#include "CANLibrary.h"
#include "CANScience.h"
#include "PCA9685.h"
#include "project.h"

#define SERVO1 0 
#define SERVO2 1
#define SERVO3 2
#define SERVO4 3
#define SERVO5 4 
#define SERVO6 5
#define SERVO7 6
#define SERVO8 7
#define SERVO9 8
#define SERVO10 9

#define SERVO_COUNT 10
#define SERVO_MAX_ANGLE 180
#define SERVO_MIN_ANGLE 0

int set_servo_position(int servo, int degrees);

/* [] END OF FILE */
