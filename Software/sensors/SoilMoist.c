/*
 * SoilMoist.c
 *
 *  Created on: 11-mrt.-2020
 *      Author: Arne de Brabanter
 *      Description: code for controlling soil moisture sensor
 */

#include <stdbool.h>
#include <stdlib.h>
#include <system.h>
#include <SoilMoist.h>
#include <dbprint.h>
#include <em_cmu.h>
#include <em_adc.h>

// ADC - if using analog soil moisture sensor
#define adcFreq   16000000
#define ADC_SE_VFS              (float)3.3              /* AVDD */

/*
 * measure soil moisture of analog sensor
 */
void measure_soil_humidity() {
	volatile uint32_t sample;
	volatile uint32_t millivolts;
	volatile float percent;

	// Start ADC conversion
	ADC_Start(ADC0, adcStartSingle);

	// Wait for conversion to be complete
	while(!(ADC0->STATUS & _ADC_STATUS_SINGLEDV_MASK));

	// Get ADC result
	sample = ADC_DataSingleGet(ADC0); // sample = value between 0-4095

	// Calculate input voltage in mV
	millivolts = (sample * ADC_SE_VFS*1000) / 4096; //4096 = 12 bit resolutie & 3300mV
	percent = (millivolts*100) / 2500; //100 percent
}
/*
 * Measure soil moisture of digital sensor
 */
double measure_soil_IIC(){
	uint8_t wBuffer[2];
	wBuffer[0] = SOIL_REG_RESULT;
	wBuffer[1] =  0x00;
	uint8_t rBuffer[2];
	IIC_WriteReadBuffer(SOIL_IIC_ADDRESS<<1, wBuffer, 1, rBuffer, 2);
	// Ref voltage: 3V
	// max voltage: 2.8V --> 2,8V/(3V/2^8)= 0xEB
	// min voltage: 1.4V --> 1,4V(3V/2^8) = 0x70
	// Not whole range in use

	uint16_t readfinal = ((uint16_t)rBuffer[0] << 4) |(uint16_t) rBuffer[1] >> 4; // combine
	double final =readfinal - 0x70; 	 //offset
	double procent = (final/123)*100;	//calculate to percent
	return 100 - procent;
}


