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

#ifndef __FSM__
#define __FSM__
    
/*
  Using modes with states so that the FSM will not
  need a separate path for building the data, but 
  instead can read data generally then use the mode
  to figure out how to apply it.
*/

// STATE DECLARATIONS
#define IDLE            (uint8_t) 0x0
#define READ_SET_DATA   (uint8_t) 0x1
#define SERVO_SELECT    (uint8_t) 0x2    
// MODE DECLARATIONS
#define MICHAEL_MODE        (uint8_t) 0x0 // default mode
#define LAZY_SUSAN_MODE     (uint8_t) 0x1
#define SCI_SERVO_MODE      (uint8_t) 0x2
#define CONT_SERVO_MODE     (uint8_t) 0x3
    
void UART_FSM(char rxByte);

void setFSMState(uint8_t nextState);

void setFSMMode(uint8_t nextMode);

// > 0 --> positive, 0 --> negative
void setSign(uint8_t sign);

void setVal(uint32_t nextVal);

void setFlag(uint8_t nextFlag);

uint32_t getVal();

uint8_t getServoID();

uint8_t getFlag();

void resetFlag();

void resetFSM();

void resetAllServos();

// State Actions

void idleAction(char rxByte);

void readDataAction(char rxByte);

void servoSelectAction(char rxByte);

void setScienceServo();

void setContinuousScienceServo();

#endif // __FSM__

/* [] END OF FILE */
