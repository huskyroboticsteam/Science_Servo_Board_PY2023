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

#define CAM1_ID 1 
#define CAM2_ID 2

void set_servo_position(uint8_t servo, uint8_t degrees);
void set_servo_continuous(int servo, int power);
void correctPosition(int servo, int power);
uint32_t cups_forward (uint8_t goal_cup_pos, uint32_t current_cup_pos);
void reset_servo_cont(); 
/* [] END OF FILE */
