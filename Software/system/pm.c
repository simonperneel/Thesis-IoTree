/*
 *  *  Created on: 17-03-2019
 *      Author: Simon Perneel
 *  Description: Power management functions. Allows you to enable/disable
 *   			 LoRa communication module.
 */

#include <em_gpio.h>

#include "pm.h"
#include "pin_mapping.h"
#include "delay.h"

void PM_Init(void) {
	GPIO_PinModeSet(PM_RN2483_PORT, PM_RN2483_PIN, gpioModePushPull, 0);
}

void PM_Enable(PM_SubSystem_t pmss) {
	GPIO_PinOutSet(PM_RN2483_PORT, PM_RN2483_PIN);
}

void PM_Disable(PM_SubSystem_t pmss) {
	GPIO_PinOutClear(PM_RN2483_PORT, PM_RN2483_PIN);
}
