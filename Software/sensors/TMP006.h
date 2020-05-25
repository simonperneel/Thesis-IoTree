/*
 * TMP006.h
 *
 *  Created on: 15-okt.-2019
 *      Author: Arne de Brabanter
 *      Description: header file for TMP006.c
 */

#ifndef INC_TMP006_H_
#define INC_TMP006_H_

#include <stdbool.h>

#define TMP006_IIC_ADDRESS 0x44 //10000010 --> VCC is connected with ADR1

#define TMP006_VOLT_OBJECT 0x00 //Read-only & 2-complement
#define TMP006_TEMP 0x01 //Read-only
#define TMP006_CONFIG 0x02 //0b 0111 0100 0000 0000
#define TMP006_MANUFACT_ID 0xFE //5449h
#define TMP006_DEVICE_ID 0xFF //0067h

#define TMP006_B0 -0.0000294
#define TMP006_B1 -0.00000057
#define TMP006_B2 0.00000000463
#define TMP006_C2 13.4
#define TMP006_TREF 298.15
#define TMP006_A2 -0.00001678
#define TMP006_A1 0.00175

#define TMP006_S0 6.4 // * 10^-14 --> change value for calibration

void start_boomTemp();
void sleep_boomTemp();
void wake_boomTemp();
double calculateboomTemp();

#endif /* INC_TMP006_H_ */


