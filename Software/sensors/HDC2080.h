/*
 * HDC2080.h
 *
 *  Created on: 10-okt.-2019
 *      Author: arned
 */

#ifndef INC_HDC2080_H_
#define INC_HDC2080_H_

#include <stdbool.h>


/****************** REGISTERS  ********************************/
/* HDC2080 I2C Address & HW ID */
#define HDC2080_IIC_ADDRESS		0x40
#define HDC2080_HW_ID_CODE_LOW		0x00D0
#define HDC2080_HW_ID_CODE_HIGH		0x0007
#define HDC2080_HW_ID_CODE			0xD007


#define HDC2080_TEMP_LOW								0x00
#define HDC2080_TEMP_HIGH								0x01
#define HDC2080_HUM_LOW 								0x02
#define HDC2080_HUM_HIGH 								0x03
#define HDC2080_INT 									0x04
#define HDC2080_TEMP_MAX								0x05
#define HDC2080_HUM_MAX									0x06
#define HDC2080_INT_ENABLE 								0x07
#define HDC2080_TEMP_OFFSET								0x08
#define HDC2080_HUM_OFFSET 								0x09
#define HDC2080_TEMP_THR_LOW 							0x0A
#define HDC2080_TEMP_THR_HIGH 							0x0B
#define HDC2080_RH_THR_LOW 								0x0C
#define HDC2080_RH_THR_HIGH 							0x0D
#define HDC2080_RESET_DRDY_INT_CONFIG					0x0E
#define HDC2080_MEASURE_CONFIG							0x0F
#define HDC2080_MANUFACT_ID_LOW							0xFC
#define HDC2080_MANUFACT_ID_HIGH						0xFD
#define HDC2080_HW_ID_LOW								0xFE
#define HDC2080_HW_ID_HIGH								0xFF
#define HDC2080_HW

void init_globTemp();
void start_globTemp();
void measureGlobTemp();

#endif /* INC_HDC2080_H_ */
