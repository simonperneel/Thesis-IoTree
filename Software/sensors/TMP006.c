/*
 * TMP006.c
 *
 *  Created on: 11-mrt.-2020
 *      Author: Arne de Brabanter
 *      Description: code for controlling TMP006 IR-sensor
 */

#include <stdbool.h>
#include <stdlib.h>
#include "pin_mapping.h"
#include <system.h>
#include <TMP006.h>

/*
 * Configuration IR sensor
 */
void start_boomTemp(){
	uint8_t wBuffer[3];
	wBuffer[0] = TMP006_CONFIG; //configuratie register
	//wBuffer[1] = 0x74; // 0b 0111 0100 default
	wBuffer[1] = 0x78; // 0b 0111 1000
	wBuffer[2] = 0x00; // 0b 0000 0000
	IIC_WriteBuffer(TMP006_IIC_ADDRESS<<1, wBuffer, 3); //
}
/*
 * Sleep IR sensor in config register
 */
void sleep_temp(){
	uint8_t wBuffer[3];
	wBuffer[0] = TMP006_CONFIG; //configuratie register
	wBuffer[1] = 0x00; // 0b 0000 0000 power down
	wBuffer[2] = 0x00; // 0b 0000 0000
	IIC_WriteBuffer(TMP006_IIC_ADDRESS<<1, wBuffer, 3); //
}
/*
 * Wake IR sensor in config register
 */
void wake_boomTemp(){
	uint8_t wBuffer[3];
	wBuffer[0] = TMP006_CONFIG; //configuratie register
	wBuffer[1] = 0x70; // 0b 0111 0000 power up
	wBuffer[2] = 0x00; // 0b 0000 0000
	IIC_WriteBuffer(TMP006_IIC_ADDRESS<<1, wBuffer, 3); //
}
/**
 * Calculate measured IR values
 * calculations were done based on datasheets
 */
double calculateboomTemp(){
	uint16_t Tdie;
	//readDieTemperature
	uint8_t rBufferTemp[2];
	uint8_t wBufferTemp[1];
	wBufferTemp[0]= TMP006_TEMP;
	IIC_WriteReadBuffer(TMP006_IIC_ADDRESS<<1, wBufferTemp, 1 , rBufferTemp, 2);
	// combinate in uint16
	Tdie = rBufferTemp[0];
	Tdie = Tdie << 8;//MSB
	Tdie |= rBufferTemp[1];//LSB
	Tdie >>= 2; //right-shift 2 LSB bits (2comp)

	//readRawVoltage
	uint8_t rBufferVolt[2];
	uint8_t wBufferVolt[1];
	wBufferVolt[0]= TMP006_VOLT_OBJECT;
	IIC_WriteReadBuffer(TMP006_IIC_ADDRESS<<1, wBufferVolt, 1 , rBufferVolt, 2);
	// combinate in uint16
	uint16_t Vobj;
	Vobj = rBufferVolt[0];
	Vobj = Vobj << 8;
	Vobj |= rBufferVolt[1];
	int Vojbect = Vobj;
	float Tchip = Tdie;

	Vojbect *= 156.25;  // 156.25 nV per LSB
	Vojbect /= 1000; // nV -> uV
	Vojbect /= 1000; // uV -> mV
	Vojbect /= 1000; // mV -> V
	Tchip *= 0.03125; // convert to celsius
	Tchip += 273.15; // convert to kelvin

	float tdie_tref = Tchip - TMP006_TREF;
	float S = (1 + (TMP006_A1*tdie_tref) + (TMP006_A2*(tdie_tref*tdie_tref)));
	S *= TMP006_S0;
	S /= 10000000;
	S /= 10000000; // 10^-14

	float Vos = TMP006_B0 + (TMP006_B1*tdie_tref) + (TMP006_B2*(tdie_tref*tdie_tref));
	float fVobj = (Vojbect - Vos) + TMP006_C2*((Vojbect-Vos)*(Vojbect-Vos));
	float Tobj = sqrt(sqrt((Tchip * Tchip * Tchip * Tchip) + (fVobj/S)));
	Tobj -= 273.15; // Kelvin -> *C
	return Tobj-4; // offset
}

