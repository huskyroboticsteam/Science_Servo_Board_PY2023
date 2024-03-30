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
#include "FSM_Stuff.h"
#include "HindsightCAN/CANLibrary.h"
#include "Port.h"
#include "PCA9685.h"
#include "cam_servo.h"


// LED stuff
volatile uint8_t CAN_time_LED = 0;
volatile uint8_t ERROR_time_LED = 0;

// UART stuff
char txData[TX_DATA_SIZE];

// CAN stuff
CANPacket received;
uint8 address = 0;

CY_ISR(Period_Reset_Handler) {
    CAN_time_LED++;
    ERROR_time_LED++;

    if (ERROR_time_LED >= 3) {
        LED_ERR_Write(OFF);
    }
    if (CAN_time_LED >= 3) {
        LED_CAN_Write(OFF);
    }
}

CY_ISR(Button_1_Handler) {
    LED_DBG_Write(!LED_DBG_Read());
}

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
    
    LED_DBG_Write(0);
    
    InitCAN(DEVICE_GROUP_GPIO_BOARDS, (int)address); // Board group as GPIO?
    Timer_Period_Reset_Start();
    
    pca_init();

    isr_Button_1_StartEx(Button_1_Handler);
    isr_Period_Reset_StartEx(Period_Reset_Handler);
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



void DisplayErrorCode(uint8_t code) {    
    ERROR_time_LED = 0;
    LED_ERR_Write(ON);
    
    sprintf(txData, "Error %X\r\n", code);
    Print(txData);

    switch(code)
    {
        case ERROR_INVALID_TTC:
            Print("Cannot Send That Data Type!\n\r");
            break;
        default:
            //some error
            break;
    }
}

/* [] END OF FILE */
