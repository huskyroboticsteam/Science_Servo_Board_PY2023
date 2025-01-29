/* File:         sensor.c
 * Authors:      Oliver Huang, Jordan Smith.
 * Organization: Husky Robotics Team
 * 
 * This file includes fuction implementations for the sensors on the 
 * Science Board allowing reading from an ADC
 */
#include "project.h"
#include <math.h>
#include "CANLibrary.h"
#include "CANScience.h"
#include "sensor.h"
#include <stdint.h>
#include "PCA9685.h"
#include <stdio.h>
#include "debug.h"


#define VEML6070_ADDR_ARA 0x18//(0x18 >> 1)
#define VEML6070_ADDR_CMD 0x70//(0x70 >> 1)
#define VEML6070_ADDR_DATA_LSB 0x71 //(0x71 >> 1)
#define VEML6070_ADDR_DATA_MSB 0x73 //(0x73 >> 1)
#define TGS2611_SLOPE_CHAN 0
#define TGS2611_R0 6700
#define TGS2611_R1 2200
#define TGS2611_YINT 63 // calibrate this
#define TGS2611_SLOPE -0.4


//referenced John's past work
void get_data(uint8_t sensor_type, uint8_t target_group, uint8_t target_serial) {
    CANPacket new_packet;
    uint32_t sensor_val = 0;
    switch(sensor_type) {
        case CAN_SCIENCE_SENSOR_MOISTURE :
            sensor_val = read_ADC(0);
            break;
        case CAN_SCIENCE_SENSOR_TEMPERATURE :
            sensor_val = read_ADC(1); 
            break;
        case CAN_SCIENCE_SENSOR_UV :
            sensor_val = read_uv_sensor();
            break;
    }
    //assemble and send telemetry packet
    AssembleTelemetryReportPacket(&new_packet, target_group, target_serial, sensor_type, sensor_val);
    //SendCANPacket returns 0x0 for successful send, 0x1 for generic error, 0x2 if all output buffers are full
    if(SendCANPacket(&new_packet)){ 
		ERR_LED_Write(0);
        //CyDelay(500);
        ERR_LED_Write(1);
	}
    
}

void periodicSend() {
    get_data(CAN_SCIENCE_SENSOR_MOISTURE, DEVICE_GROUP_SCIENCE, DEVICE_SERIAL_SCIENCE_STATION);
    get_data(CAN_SCIENCE_SENSOR_TEMPERATURE, DEVICE_GROUP_SCIENCE, DEVICE_SERIAL_SCIENCE_STATION);
    get_data(CAN_SCIENCE_SENSOR_UV, DEVICE_GROUP_SCIENCE, DEVICE_SERIAL_SCIENCE_STATION);
}

uint16_t read_ADC(uint32_t channel) {
    ADC_StartConvert();
    ADC_IsEndConversion(ADC_WAIT_FOR_RESULT); //returns when all conversions are completed
    ADC_StopConvert();
    return ADC_GetResult16(channel); //channel 0 for humidity and 1 for temperature
}

uint32_t readKeroseneSensor() {
    uint16 N = 200;
    uint32 sum = 0;
    
    // Change the range of the loop based on speed of the ADC reading 
    for(int i = 0; i < N; i++) {
        sum += read_ADC(TGS2611_SLOPE_CHAN);
    }
    
    char txData[200];
    
    float32 adc_avg = sum / N;
    
    float32 Rs = (TGS2611_R1 * adc_avg) / (2048 - adc_avg);

    float32 ratio = Rs/TGS2611_R0;
    
    float32 concentration = pow((ratio / TGS2611_YINT), (1.0/TGS2611_SLOPE));
    sprintf(txData, "The concentration is %d ppm\r\n", (int)(concentration));
    Print(txData);
    return concentration; 
} 
    
void VEML6070_init(){
    I2C_Start();
    I2C_Enable();
    uint8_t address;
    uint8_t error;
    uint8_t data[1] = {0x06};
    uint8 mode_buf[2] = {0x6,0x6};
    I2C_I2CMasterClearStatus(); //clear the garbage
    I2C_bus_read(VEML6070_ADDR_ARA, VEML6070_ADDR_ARA, &address, 1);
//    I2C_I2CMasterSendStart(VEML6070_ADDR_ARA, I2C_I2C_READ_XFER_MODE, 1);
//    I2C_I2CMasterReadByte(I2C_I2C_ACK_DATA, &address, 100); //read ARA to clear interrupt (ACK)
//    I2C_I2CMasterSendStop(20); 
    
    writeBuffer(mode_buf, 0x70);//VEML6070_ADDR_CMD);
       
    
//    I2C_I2CMasterSendStart(VEML6070_ADDR_CMD, I2C_I2C_WRITE_XFER_MODE, 1);
//    //I2C_I2CMasterWriteByte(VEML6070_ADDR_CMD, 20);
//    I2C_I2CMasterWriteByte(0x6, 20);  //integration time
//    I2C_I2CMasterSendStop(10);

    
//    I2C_bus_read(VEML6070_ADDR_CMD, VEML6070_ADDR_ARA, &address, 1);
//    I2C_bus_write(VEML6070_ADDR_CMD, VEML6070_ADDR_CMD, data, 1);   
//    I2C_I2CMasterSendStart(VEML6070_ADDR_CMD, I2C_I2C_WRITE_XFER_MODE, 1);
//    I2C_I2CMasterWriteByte(VEML6070_ADDR_CMD, 20);  //0x70
//    I2C_I2CMasterWriteByte(0x6, 1);  //wrByte, timeoutms
//    I2C_I2CMasterSendStop(10);
    
}

uint16_t read_uv_sensor() {
    I2C_I2CMasterClearStatus(); //clear the garbage
    uint8_t msb, lsb;
/*    
    I2C_I2CMasterSendStart(VEML6070_ADDR_DATA_MSB, I2C_I2C_READ_XFER_MODE, 1);
    //I2C_I2CMasterWriteByte(VEML6070_ADDR_DATA_MSB, 20);
    I2C_I2CMasterReadByte(I2C_I2C_ACK_DATA, &msb, 100);
    I2C_I2CMasterSendStop(20);
//    
    I2C_I2CMasterSendStart(VEML6070_ADDR_DATA_LSB, I2C_I2C_READ_XFER_MODE, 1);
    //I2C_I2CMasterWriteByte(VEML6070_ADDR_DATA_LSB, 20);
    I2C_I2CMasterReadByte(I2C_I2C_ACK_DATA, &lsb, 100);
    I2C_I2CMasterSendStop(20);
*/
     uint8_t msberror = I2C_bus_read(VEML6070_ADDR_DATA_MSB, VEML6070_ADDR_DATA_MSB, &msb, 0);
     uint8_t lsberror = I2C_bus_read(VEML6070_ADDR_DATA_LSB, VEML6070_ADDR_DATA_LSB, &lsb, 0);

    //if (lsberror != I2C_I2C_MSTR_NO_ERROR || msberror != I2C_I2C_MSTR_NO_ERROR){
     //   ERR_LED_Write(0);
       // CyDelay(500);
        //ERR_LED_Write(1);
    //}
	return (((uint16_t) msb) << 8) | (uint16_t) lsb;
}



//yoinked from davis (imu)
//read byte(s) from i2C and store in array
uint8_t I2C_bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt) {
    I2C_I2CMasterClearStatus(); //clear the garbage
  
    int ms_timeout = 20; 
    int32_t BNO055_iERROR = 0; // this is the "status" we usually use in our R/W functions 
	uint8_t idx;
//	BNO055_iERROR = I2C_I2CMasterSendStart(dev_addr, I2C_I2C_WRITE_XFER_MODE, ms_timeout);
	// Check for BNO055_iERROR before proceeding
//	BNO055_iERROR = I2C_I2CMasterWriteByte(reg_addr, ms_timeout);
	// Check for BNO055_iERROR before proceeding
//	BNO055_iERROR = I2C_I2CMasterSendStop(10);
	
	BNO055_iERROR = I2C_I2CMasterSendStart(dev_addr, I2C_I2C_READ_XFER_MODE, ms_timeout);
    //PrintInt(BNO055_iERROR);
	for (idx = 0; (idx < cnt) && (BNO055_iERROR == 0); idx++)
	{
        if (idx < cnt-1)
        {
		    I2C_I2CMasterReadByte(I2C_I2C_ACK_DATA, &reg_data[idx], ms_timeout);
        }
        else
        {
            I2C_I2CMasterReadByte(I2C_I2C_NAK_DATA, &reg_data[idx], ms_timeout);
        }
	}
	// Check for BNO055_iERROR before proceeding
	BNO055_iERROR = I2C_I2CMasterSendStop(ms_timeout);
    
	return (int8_t)BNO055_iERROR;
}

//yoinked from davis (imu)
uint8_t I2C_bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt) {
    //Print("writing");
    I2C_I2CMasterClearStatus(); //clear the garbage
    uint8_t data_pack [cnt+1]; 
    data_pack[0] = reg_addr;
    for(int i = 1; i < cnt+1; i++){
        data_pack[i] = reg_data[i-1];   
    }
    int status = I2C_I2CMasterWriteBuf(dev_addr, data_pack, cnt+1, I2C_I2C_MODE_COMPLETE_XFER);
    while ((I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT) == 0u) //should wait for write buffer to complete
    {
        //Print("\r\nWRITE TO: \n\r");
        //PrintChar(dev_addr);
    }
    
	return status;  
}
/* [] END OF FILE */
