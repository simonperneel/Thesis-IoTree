/*
 * @file led.c
 * @brief Basic functions to control the user LED.
 * @author Simon Perneel
 */

#include <em_gpio.h>
#include <led.h>
#include <delay.h>
#include <em_cmu.h>
#include <pin_mapping.h>

int errorNr;
bool LED_initialized = false;

void LED_Init(LED_state_t initState){
	GPIO_PinModeSet(LED_PORT, LED_PIN, gpioModePushPull, initState);
}

void LED_On(){
	GPIO_PinOutSet(LED_PORT, LED_PIN);
}

void LED_Off(){
	GPIO_PinOutClear(LED_PORT, LED_PIN);
}

void LED_Toggle(){
	GPIO_PinOutToggle(LED_PORT, LED_PIN);
}
void LED_ERROR(uint8_t err){
	errorNr = err;
	for(int i=0; i<10; i++){
		delay(200);
		LED_Toggle();
	}
}
void LED_HALTED(void){
	while(1){
		delay(2000);
		LED_Toggle();
	}
}

void led (bool enabled)
{
	/* Initialize LED if not already the case */
	if (!LED_initialized) initLED();

	/* Set the selected state */
	if (enabled) GPIO_PinOutSet(LED_PORT, LED_PIN);
	else GPIO_PinOutClear(LED_PORT, LED_PIN);
}


void initLED (void)
{
	/* Enable necessary clocks (just in case) */
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_GPIO, true);

	/* In the case of gpioModePushPull, the last argument directly sets the pin state */
	GPIO_PinModeSet(LED_PORT, LED_PIN, gpioModePushPull, 0);

	LED_initialized = true;
}
