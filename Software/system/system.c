/*
 * system.c
 *
 *  Created on: 17-03-2019
 *      Author: Simon Perneel
 *      Description: system initialisation functions
 */

#include <stdbool.h>
#include <em_device.h>
#include <em_chip.h>
#include <em_cmu.h>
#include <em_emu.h>
#include <em_rmu.h>
#include <em_leuart.h>
#include <rtcdriver.h>
#include "rtcdrv_config.h"
#include <gpio.h>
#include "system.h"
#include "pin_mapping.h"
#include "pm.h"
#include <em_adc.h>


void System_Init(void){

	/* Chip errata */
	CHIP_Init();

	// Initialize user LED
	LED_Init(LED_OFF);

	// Initialize power supply management
	PM_Init();

	// Initialize GPIO pins
	init_GPIO();

	// Inialize ADC
	init_ADC();

	// Initialize I2C bus (to interface with sensors)
	IIC_Init();

	//Start clock
	setupOscillator();
}

/****************************************************************************
 * @brief  Starting oscillators and enabling clocks
 *****************************************************************************/
void setupOscillator(void)
{
	/* Enabling clock to the IIC, GPIO, LE */
	CMU_ClockEnable(cmuClock_I2C0, true);


	/* Starting LFXO and waiting until it is stable */
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);

	/* Enable necessary clocks (just in case) */
	CMU_ClockEnable(cmuClock_HFPER, true); /* GPIO is a High Frequency Peripheral */
	CMU_ClockEnable(cmuClock_GPIO, true);
}

/***************************************************************************//**
 * @brief initialize ADC input for analog soil moisture sensor
 * @param
 ******************************************************************************/
#define adcFreq   16000000
#define ADC_SE_VFS              (float)3.3              /* AVDD */
void init_ADC (void)

{
	CMU_ClockEnable(cmuClock_HFPER, true); /* ADC0 is a High Frequency Peripheral */
	// Enable ADC0 clock
	CMU_ClockEnable(cmuClock_ADC0, true);
	// Declare init structs
	ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
	ADC_InitSingle_TypeDef initSingle = ADC_INITSINGLE_DEFAULT;

	// Modify init structs and initialize
	init.prescale = ADC_PrescaleCalc(adcFreq, 0); // Init to max ADC clock for Series 0
	initSingle.reference  = adcRefVDD;    // internal 3.3V reference

	/* Set a timebase providing at least 1 us.
	* If the argument is "0" the currently defined HFPER clock setting is used. */
	init.timebase = ADC_TimebaseCalc(0);

	// Select ADC input. See README for corresponding EXP header pin.
	initSingle.input = adcSingleInputCh6; // PD6
	ADC_Init(ADC0, &init);
	ADC_InitSingle(ADC0, &initSingle);
}
