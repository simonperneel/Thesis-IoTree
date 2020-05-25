/*
 * gpio.c
 *
 *  Created on: 2-okt.-2019
 *      Author: arned
 */
#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_rtc.h"

#include "pin_mapping.h"
#include "gpio.h"

/*
 * Power/disable power of GPIO
 * @parameters:
 * true = enable power, false = disable power
 * port and pin of specific GPIO
 */
void powerSensor(bool enable, GPIO_Port_TypeDef port, unsigned int pin){
	if(enable){
		GPIO_PinOutSet(port, pin);
	}
	else{
		GPIO_PinOutClear(port, pin);
	}
}
/*
 * initialize sensor power pins and interrupt accelerometer
 */
void init_GPIO(){
	/* Enable necessary clocks (just in case) */
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_DriveModeSet(gpioPortC, gpioDriveModeStandard);
	//init pin and disable power
	GPIO_PinModeSet(HDC2080_PORT, HDC2080_PIN, gpioModePushPull, 0); // temperature & humidity sensor
	GPIO_PinModeSet(TMP006_PORT, TMP006_PIN, gpioModePushPull, 0); // tree temperature sensor
	GPIO_PinModeSet(SOIL_PORT, SOIL_PIN, gpioModePushPull, 0); // Soil Moisture sensor
	GPIO_PinModeSet(CCS811_PORT, CCS811_PIN, gpioModePushPull, 0); // CO2 sensor
	GPIO_PinModeSet(LIS3DH_PORT, LIS3DH_PIN, gpioModePushPull, 0); // Accelerometer sensor
	GPIO_PinModeSet(PM_RN2483_PORT, PM_RN2483_PIN, gpioModePushPull, 0); // LoRa modem

	/* Clear all odd pin interrupt flags (just in case)*/
	GPIO_IntClear(0xAAAA);

	/* Clear all even pin interrupt flags (just in case) */
	GPIO_IntClear(0x5555);
}
