/*
 * system.c
 *
 *  Created on: 17-03.-2019
 *      Author: Simon Perneel
 *      Description: header file for system.c
 */
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "delay.h"
#include "led.h"
#include "iic.h"

void System_Init(void);
void setupOscillator(void);
void init_ADC(void);

#endif /* _SYSTEM_H_ */
