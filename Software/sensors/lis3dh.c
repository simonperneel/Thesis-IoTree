/*
 * lis3dh.c
 *
 *  Created on: 11-mrt.-2020
 *     Arne de Brabanter & Simon Perneel
 *     v1.0: measure accelerations on the axes
 *     v1.1: added functionality to measure tilt
 */

#include <stdbool.h>
#include <stdlib.h>
#include "pin_mapping.h"
#include <system.h>
#include <lis3dh.h>
#include <dbprint.h>
#include <debug_dbprint.h>
#include <em_cmu.h>
#include "math.h"
#include <delay.h>

/************ADJUSTABLE********************************************************/
#define THRESHOLD_INT 0x04// 1LSB = 16mg ;   0x07
/******************************************************************************/
extern bool interrupt_triggered;
extern float maxtilt;
uint16_t x_accel, y_accel, z_accel;


/*
 * Accelerometer initialization
 */
void init_accelerometer(){
	// Enable all axes, low power mode, 0 datarate
	uint8_t wBuffer[2];
	wBuffer[0] = LIS3DH_REG_CTRL1;
	wBuffer[1] =  0x0F; //0F voor low power mode
	IIC_WriteBuffer(LIS3DH_IIC_ADDRESS<<1, wBuffer, 2);

	// Enable continuous updates, enable high resolution output, 2g
	// other sensitivities: 2g=0x08, 4g=0x18, 8g=0x28, 16g= 0x38
	wBuffer[0] = LIS3DH_REG_CTRL4;
	wBuffer[1] = 0x08;
	IIC_WriteBuffer(LIS3DH_IIC_ADDRESS<<1, wBuffer, 2);

	//READ REG_CTRL1 in rBuffer[0]
	wBuffer[0] = LIS3DH_REG_CTRL1;
	wBuffer[1] =  0x00;
	uint8_t rBuffer[1];
	IIC_WriteReadBuffer(LIS3DH_IIC_ADDRESS<<1, wBuffer, 1, rBuffer, 1);

	//set datarate 10Hz
	uint8_t dr = LIS3DH_DATARATE_10_HZ;
	uint8_t ctl1 = rBuffer[0];
	ctl1 &= (0x0F); // mask off bits
	ctl1 |= (dr << 4);
	wBuffer[0] = LIS3DH_REG_CTRL1;
	wBuffer[1] = ctl1;
	IIC_WriteReadBuffer(LIS3DH_IIC_ADDRESS<<1, wBuffer, 2, rBuffer, 1);
}
/*
 * initialization interrupt accelerometer
 */
void init_interrupt_accel(){
	interrupt_triggered =false;
	// Enable IA1 to INT1
	uint8_t wBuffer[2];
	wBuffer[0] = LIS3DH_REG_CTRL3;
	wBuffer[1] = 0x40;
	IIC_WriteBuffer(LIS3DH_IIC_ADDRESS<<1, wBuffer, 2);

	// Enable interrupt on Y and X high values
	wBuffer[0] = LIS3DH_REG_INT1CFG;
	wBuffer[1] = 0x0A;
	IIC_WriteBuffer(LIS3DH_IIC_ADDRESS<<1, wBuffer, 2);

	// Clear interrupt sources
	wBuffer[0] = LIS3DH_REG_INT1SRC;
	wBuffer[1] = 0x00;
	IIC_WriteBuffer(LIS3DH_IIC_ADDRESS<<1, wBuffer, 2);

	//low power
	// 2g threshold 1 LSB=16mg
	// 4g threshold 1 LSB=32mg
	// 8g threshold 1 LSB=64mg
	// 16g threshold 1 LSB=192mg
	wBuffer[0] = LIS3DH_REG_INT1THS;
	wBuffer[1] = THRESHOLD_INT;
	IIC_WriteBuffer(LIS3DH_IIC_ADDRESS<<1, wBuffer, 2);

	// Clear minimum duration
	wBuffer[0] = LIS3DH_REG_INT1DUR;
	wBuffer[1] = 0x04;
	IIC_WriteBuffer(LIS3DH_IIC_ADDRESS<<1, wBuffer, 2);
	/* Enable necessary clocks (just in case) */
	//CMU_ClockEnable(cmuClock_HFPER, true); /* GPIO is a High Frequency Peripheral */
	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(LIS3DH_INT_PORT, LIS3DH_INT_PIN, gpioModeInput, 1);
	/* Clear all odd pin interrupt flags (just in case)*/
	GPIO_IntClear(0xAAAA);
	/* Enable IRQ for odd numbered GPIO pins */
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
	/* Enable rising-edge interrupts for LIS3DH_INT1 */
	GPIO_ExtIntConfig(LIS3DH_INT_PORT, LIS3DH_INT_PIN, LIS3DH_INT_PIN, true, false, true);
	}
/*
 * measure accelerometer data
 */
void measure_Accelerometer() {
	uint8_t rBuffer[6];
	uint8_t wBuffer[1] = { LIS3DH_REG_OUT_X_L | 0x80 }; // 0x80 for autoincrement
	IIC_WriteReadBuffer(LIS3DH_IIC_ADDRESS << 1, wBuffer, 1, rBuffer, 6);

	x_accel = rBuffer[0] | rBuffer[1] << 8;
	y_accel = rBuffer[2] | rBuffer[3] << 8;
	z_accel = rBuffer[4] | rBuffer[5] << 8;

	//low power mode: shift 8 bits (8-bit resolutie code) * sensitivity (16mg/LSB)
	//Low Power Mode: { 2g = 16mg/digit, 4g = 32mg/digit, 8g = 64mg/digit, 16g = 192mg/digit }
	//normal power mode: shift 6 bits (10-bit resolutie code) * sensitivity (4mg/LSB)
	// High res mode { 2g = 1mg/digit,   4g = 2mg/digit,   8g = 4mg/digit,   16g = 12mg/digit }
	// High res: 4 bits shiften * sensitivity (1mg/digit)
	float outputx = (float) (x_accel >> 4) * 1;
	float outputy = (float) (y_accel >> 4) * 1;
	float outputz = (float) (z_accel >> 4) * 1; // acceleration in mg


	// voor 2G
	if (outputx > 2040) {
		outputx -= 4080;
	}
	if (outputy > 2040) {
		outputy -= 4080;
	}
	if (outputz > 2040) {
		outputz -= 4080;
	}
	float xversnel= outputx*0.00981; // acceleration in m/s²
	float yversnel= outputy*0.00981;
	float zversnel= outputz*0.00981;
	/* calculate inclination */
	double inclination = fabs(atan(sqrt(xversnel*xversnel+yversnel*yversnel)/zversnel)*(180/PI));

	/* keep max inclination value, values above 100° are ignored */
	if((inclination < 100) & (inclination > maxtilt)){
		maxtilt = inclination;
}

}

/**************************************************************************
 * GPIO ODD IRQ on ODD-numbered pins.
 *************************************************************************/
void GPIO_ODD_IRQHandler (void)
{
	interrupt_triggered = true;
	/* Clear all odd pin interrupt flags */
	GPIO_IntClear(0xAAAA);
}

/*
 * disable interrupt of accelerometer
 */
void Lis3dh_DisableInterruptPin(void){
	GPIO_ExtIntConfig(LIS3DH_INT_PORT, LIS3DH_INT_PIN, LIS3DH_INT_PIN, false, true, false);
	GPIO_IntClear(1	<< LIS3DH_INT_PIN);	// Clear pending interrupts
	GPIO_PinModeSet(LIS3DH_INT_PORT, LIS3DH_INT_PIN, gpioModePushPull, 0);
}
