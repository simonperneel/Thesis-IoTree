
/*
 * pin_mapping.h
 *
 *  Created on: 1-okt.-2019
 *      Author: Arne de Brabanter & Simon Perneel
 */

#ifndef INC_PIN_MAPPING_H_
#define INC_PIN_MAPPING_H_

/* Include necessary for this header file */
#include "em_device.h" /* Include necessary MCU-specific header file */
#include "em_gpio.h"   /* General Purpose IO */

#define CUSTOM_BOARD  0 //0: Happy gecko, 1: custom board


#if CUSTOM_BOARD == 0 /*  Happy Gecko pinout */
	// I2C pins
	#define SDA_PORT gpioPortD
	#define SDA_PIN 6
	#define SCL_PORT gpioPortD
	#define SCL_PIN 7
	#define PORT_LOC 1

	// voeding gpio CO2
	#define CCS811_PORT 			gpioPortA
	#define CCS811_PIN				1

	//voeding gpio TMP006
	#define TMP006_PORT 			gpioPortE
	#define TMP006_PIN				12

	//voeding gpio HDC2080
	#define HDC2080_PORT 			gpioPortE
	#define HDC2080_PIN				13

	//voeding gpio Accelerometer
	#define LIS3DH_PORT 			gpioPortE
	#define LIS3DH_PIN				10

	//Interrupt pin Accelerometer
	#define LIS3DH_INT_PORT		gpioPortE
	#define LIS3DH_INT_PIN			11

	//voeding gpio soil moisture
	#define SOIL_PORT 			gpioPortC
	#define SOIL_PIN				1

	/* LED */
	#define LED_PORT            gpioPortA
	#define LED_PIN             2

	// RN2483
	#define PM_RN2483_PORT		gpioPortA
	#define PM_RN2483_PIN		8
	#define RN2483_RESET_PORT	gpioPortA
	#define RN2483_RESET_PIN	10
	#define RN2483_RX_PORT		gpioPortD
	#define RN2483_RX_PIN		5
	#define RN2483_TX_PORT		gpioPortD
	#define RN2483_TX_PIN		4
	#define RN2483_UART			LEUART0
	#define RN2483_UART_LOC		0


#else /* Custom Happy Gecko pinout */
	// I2C pins
	#define SDA_PORT 		gpioPortC
	#define SDA_PIN 		0
	#define SCL_PORT 		gpioPortC
	#define SCL_PIN 		1
	#define PORT_LOC 		4

	// voeding gpio CO2
	#define CCS811_PORT 	gpioPortC
	#define CCS811_PIN		2

	//voeding gpio boomtemp
	#define TMP006_PORT 	gpioPortC
	#define TMP006_PIN		3

	//voeding gpio humidity and temperature
	#define HDC2080_PORT 	gpioPortC
	#define HDC2080_PIN		4

	//voeding gpio Accelerometer
	#define LIS3DH_PORT 	gpioPortA
	#define LIS3DH_PIN		10

	//Interrupt PA9 Accelerometer
	#define LIS3DH_INT_PORT	gpioPortA
	#define LIS3DH_INT_PIN	9

	//voeding gpio soil moisture
	#define SOIL_PORT 		gpioPortA
	#define SOIL_PIN		8

	/* LED */
	#define LED_PORT		gpioPortB
	#define LED_PIN			11

	// RN2483
	#define PM_RN2483_PORT		gpioPortD
	#define PM_RN2483_PIN		7
	#define RN2483_RESET_PORT	gpioPortC
	#define RN2483_RESET_PIN	8
	#define RN2483_RX_PORT		gpioPortD
	#define RN2483_RX_PIN		5
	#define RN2483_TX_PORT		gpioPortD  //custom board PATCH: ook verbonden met pin D6 !!
	#define RN2483_TX_PIN		4
	#define RN2483_UART			LEUART0
	#define RN2483_UART_LOC		0




#endif /* Board pinout selection */


#endif /* _PIN_MAPPING_H_ */
