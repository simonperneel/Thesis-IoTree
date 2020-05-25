/*
 * CCS811.c
 *
 *  Created on: 11-mrt.-2020
 *      Author: Arne de Brabanter:
 *      Description: controlling the CCS811 CO2-sensor
 */

#include <CCS811.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pin_mapping.h"
#include <system.h>

/*
* write bootloader program in CCS811
*/
void start_CO2(){
	uint8_t wBuffer[1];
	wBuffer[0] = CCS811_BOOTLOADER_APP_START; //bootloader write
	IIC_WriteBuffer(CCS811_IIC_ADDRESS<<1, wBuffer, 1);
}
/*
 * Initialize CCS811
 */
void init_Co2(){
	uint8_t wBuffer[2];
	wBuffer[0] = CCS811_MEAS_MODE;
	//wBuffer[1] = 0b01000000; //set measurement mode 4: 0x40 --> continu
	//wBuffer[1] = 0b00110000; //set measurement mode 3: 0x30 --> each minute
	wBuffer[1] = 0b000100000; //set measurement mode 2: 0x20 -->  each 10 seconds
	IIC_WriteBuffer(CCS811_IIC_ADDRESS<<1,wBuffer, 2); // address, wbuffer, length
}
/*
 * Measure CO2 values
 */
uint16_t measureCo2(){
	uint8_t rBuffer[2];
	uint8_t wBuffer[1];
	wBuffer[0]= CCS811_ALG_RESULT_DATA; //CO2 data lezen
	IIC_WriteReadBuffer(CCS811_IIC_ADDRESS<<1, wBuffer, 1 , rBuffer, 2);
	return (((uint16_t) rBuffer[0]<<8) | (uint16_t) rBuffer[1]);
}
/*
 * turn off sensor in software/configregister
 */
void idle_mode(){
	uint8_t wBuffer[2];
	wBuffer[0] = CCS811_MEAS_MODE;
	wBuffer[1] = 0b00000000; //set measurement mode 0 -->turn off
	IIC_WriteBuffer(CCS811_IIC_ADDRESS<<1,wBuffer, 2); // address, wbuffer, length
}


