/*
 * SoilMoist.h
 *
 *  Created on: 17-dec.-2019
 *      Author: Arne de Brabanter
 *      Description: header file for SoilMoist.c
 */

#ifndef INC_SOILMOIST_H_
#define INC_SOILMOIST_H_

#define SOIL_IIC_ADDRESS 0x50 //10000000

//COMPONENT ADC0821

// System Defines
#define SOIL_REG_RESULT			0x00
#define SOIL_REG_ALERT			0x01
#define SOIL_REG_CONF			0x02
#define SOIL_REG_LOW_LIM		0x03
#define SOIL_REG_HIGH_LIM		0x04
#define SOIL_REG_HYSTERESIS		0x05
#define SOIL_REG_LOW_CONVR		0x06
#define SOIL_REG_HIGH_COVR		0x07

void measure_soil_humidity();
double measure_soil_IIC();

#endif /* INC_SOILMOIST_H_ */
