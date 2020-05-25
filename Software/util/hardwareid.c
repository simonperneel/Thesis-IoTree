/*
 * hardwareid.c
 *
 *      Created on: 11-mrt.-2020
 *      Author: arned
 *      Description: functionality to check if a sensor is connected by reading
 *      		     the hardware ID out if its register
 *
 */


#include <stdbool.h>
#include <stdlib.h>
#include <system.h>

/******************************************************************************
 * DYNAMIC CHECK HARDWARE ID SENSORS
 * Check if hardware ID matches the expected ID of the sensor
 *****************************************************************************/
bool checkHW_ID(uint8_t IIC_ADDRESS, uint8_t HW_ID, uint16_t verwachtteID ){
	uint8_t wBuffer[1];
	uint8_t rBuffer[1];
	wBuffer[0] = HW_ID; //HW_ID of the sensor
	IIC_WriteReadBuffer(IIC_ADDRESS<<1, wBuffer,1, rBuffer,2); //2 bytes read
	uint8_t MSB_LSB[] = { (uint8_t)(verwachtteID >> 8), (uint8_t)verwachtteID }; //divide uint16 in 2x uint8:{0xMSB,0xLSB}
	if(rBuffer[0] == MSB_LSB[0] && rBuffer[1] == MSB_LSB[1]){
		return true;	// hw_id matches
	}
	else{
		return false;
	}
}



