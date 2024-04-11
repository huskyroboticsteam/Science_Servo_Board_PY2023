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

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "cyapicallbacks.h"
#include "CAN_Stuff.h"
#include "CANScience.h"
#include "HindsightCAN/CANLibrary.h"
#include "Port.h"

// UART stuff
char txData[TX_DATA_SIZE];

// CAN stuff
CANPacket received;
uint8 address = 0x04;

// Servo Stuff


int main(void)
{ 
    Initialize();
    volatile int error;
    
    for(;;)
    {
        if (!error) {
            int ID = GetPacketID(&received);
            if (ID == ID_SCIENCE_SERVO_SET) {
                uint8_t servoID = GetScienceServoAngleFromPacket(&received);
                uint8_t angle = GetScienceServoAngleFromPacket(&received);
                set_servo_position(servoID, angle);
            }
        }
    }
}

void Initialize(void) {
    CyGlobalIntEnable; /* Enable global interrupts. LED arrays need this first */
    
    // address = getSerialAddress(); Need to choose address for this board
    
    DBG_UART_Start();
    sprintf(txData, "Dip Addr: %x \r\n", address);
    Print(txData);
    
    PWM_Start();
    
    InitCAN(DEVICE_GROUP_SCIENCE, (int)address); // Board group as Science?
}

void DebugPrint(char input) {
    switch(input) {
        case 'f':
            sprintf(txData, "Mode: %x State:%x \r\n", GetMode(), GetState());
            break;
        case 'x':
            sprintf(txData, "bruh\r\n");
            break;
        default:
            sprintf(txData, "what\r\n");
            break;
    }
    Print(txData);
}


// Given a position in degrees and servo (either CAM_SERVO 1 or 2), sets servo to given pos.
void set_servo_position(uint8_t servo, uint8_t position) {
    uint16_t pwm_duty = 50; //Set based off servo documentation
    switch(servo) {
        case(CAM_SERVO_1):        
            PWM_WriteCompare1(pwm_duty);
        case(CAM_SERVO_2):
            PWM_WriteCompare2(pwm_duty);
        default:
            sprintf(txData, "No servo found \r\n");
            Print(txData);
    }
}

/* [] END OF FILE */
