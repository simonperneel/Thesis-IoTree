/*  
 *  KU Leuven - Technology Campus Gent,
 *  Gebroeders De Smetstraat 1,
 *  B-9000 Gent, Belgium
 *
 *         File: Co2VGO_CCS811.h
 *      Created: 2019-09-23
 *       Author: Arne De Brabanter
 *
 *  Description: Header file for Co2VGO_CCS811.c
 */
#ifndef _Co2VGO_CCS811_H_
#define _Co2VGO_CCS811_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>


/****************** REGISTERS  ********************************/
/* CCS811 I2C Address & HW ID */
#define CCS811_IIC_ADDRESS		0x5B //1011011
#define CCS811_HW_ID_CODE		0x81

/* Other */
#define CCS811_STATUS 									0x00
#define CCS811_MEAS_MODE 								0x01
#define CCS811_ALG_RESULT_DATA 							0x02
#define CCS811_RAW_DATA 								0x03
#define CCS811_ENV_DATA 								0x05
#define CCS811_NTC										0x06
#define CCS811_THRESHOLDS 								0x10
#define CCS811_BASELINE 								0x11
#define CCS811_HW_ID 									0x20
#define CCS811_HW_VERSION 								0x21
#define CCS811_FW_BOOT_VERSION 							0x23
#define CCS811_FW_APP_VERSION 							0x24
#define CCS811_ERROR_ID 								0xE0
#define CCS811_SW_RESET 								0xFF

/* Bootloader */
#define CCS811_BOOTLOADER_APP_ERASE 					0xF1
#define CCS811_BOOTLOADER_APP_DATA 						0xF2
#define CCS811_BOOTLOADER_APP_VERIFY 					0xF3
#define CCS811_BOOTLOADER_APP_START 					0xF4

/* Drive Mode */
#define CCS811_DRIVE_MODE_IDLE 							0x00
#define CCS811_DRIVE_MODE_1SEC 							0x01
#define CCS811_DRIVE_MODE_10SEC 						0x02
#define CCS811_DRIVE_MODE_60SEC 						0x03
#define CCS811_DRIVE_MODE_250MS							0x04

void start_CO2();
void init_Co2();
uint16_t measureCo2();
void idle_mode();


#endif
