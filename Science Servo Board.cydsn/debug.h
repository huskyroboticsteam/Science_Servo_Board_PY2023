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

#include "project.h"
#include "CANLibrary.h"
#include <stdlib.h>

#ifndef __DEBUG__
#define __DEBUG__
    
void Print(char* message);

void PrintChar(char character);

void PrintInt(int integer);

void PrintIntBin(int integer);

void PrintCanPacket(CANPacket* packet);
    
void DebugPrint(char input);
    
#endif // __DEBUG

/* [] END OF FILE */
