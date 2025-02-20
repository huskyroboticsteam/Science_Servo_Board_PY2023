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

#include <stdio.h>
#include <stdlib.h>
#include "project.h"
#include "CANLibrary.h"

void Print(char* message) {
    DBG_UART_UartPutString(message);
}

void PrintChar(char character) {
    DBG_UART_UartPutChar(character);
}

void PrintInt(int integer) {
    char txData[200];
    DBG_UART_UartPutString(itoa(integer, txData, 10));
}

void PrintIntBin(int integer) {
    char txData[200];
    DBG_UART_UartPutString(itoa(integer, txData, 2));
}
    
void PrintCanPacket(CANPacket* packet) {
    char txData[200];
    sprintf(txData, "ID %X DLC %X DATA", packet->id, packet->dlc);
    Print(txData);
    for(int i = 0; i < packet->dlc; i++ ) {
        sprintf(txData," %02X", packet->data[i]);
        Print(txData);
    }
    Print("\r\n");
}

void DebugPrint(char input) {
    char txData[200];
    switch(input) {
        case 'p': // Position
            sprintf(txData, "Pos:%li PWM:%li", 
                1, 2);
            Print(txData);
            break;
        default:
            Print("what");
            break;
    }
    Print("\r\n");
}

/* [] END OF FILE */
