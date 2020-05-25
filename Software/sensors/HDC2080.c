/*
 * HDC2080.c
 *
 *  Created on: 10-okt.-2019
 *      Author: arned
 *      Description: code for controlling the HDC2080 Temp/Humidity sensor
 */

#include <stdbool.h>
#include <pin_mapping.h>
#include <iic.h>
#include <HDC2080.h>

extern float globalTemp;
extern float globalHum;

/*
 * Initialize HDC2080
 */

void init_globTemp(){
	uint8_t wBufferconf[2];
	wBufferconf[0] = HDC2080_MEASURE_CONFIG; //configuratie register
	wBufferconf[1] = 0x01; // 0b 0111 0000 power up
	IIC_WriteBuffer(HDC2080_IIC_ADDRESS<<1, wBufferconf, 2); //
	delay(3);
}
/*
 * configure HDC2080
 */
void start_globTemp(){
	uint8_t wBuffer[2];
	wBuffer[0] = HDC2080_RESET_DRDY_INT_CONFIG; //configregister
	wBuffer[1] = 0x00;
	IIC_WriteBuffer(HDC2080_IIC_ADDRESS<<1, wBuffer, 2);
	wBuffer[0] = HDC2080_MEASURE_CONFIG; //configregister
	wBuffer[1] = 0x00;
	IIC_WriteBuffer(HDC2080_IIC_ADDRESS<<1, wBuffer, 2);
}
/*
 * measure temperature and humidity from HDC2080
 */
void measureGlobTemp(){
	delay(2);
	uint8_t wBuffer[1];
	uint8_t rBuffer[4];
	wBuffer[0]= HDC2080_TEMP_LOW;
	IIC_WriteReadBuffer(HDC2080_IIC_ADDRESS<<1, wBuffer, 1 , rBuffer, 4);
	uint16_t temperatuurRaw = rBuffer[1];
	temperatuurRaw = temperatuurRaw <<8;
	temperatuurRaw |= rBuffer[0];
	uint16_t humidityRaw = rBuffer[3];
	humidityRaw = humidityRaw <<8;
	humidityRaw |= rBuffer[2];
	float temperatuur = temperatuurRaw;
	float humidity = humidityRaw;
	globalTemp= ((temperatuur/65536)*165)-40;
	globalHum= (humidity/65536)*100;
}
