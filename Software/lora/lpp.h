/*  ____  ____      _    __  __  ____ ___
 * |  _ \|  _ \    / \  |  \/  |/ ___/ _ \
 * | | | | |_) |  / _ \ | |\/| | |  | | | |
 * | |_| |  _ <  / ___ \| |  | | |__| |_| |
 * |____/|_| \_\/_/   \_\_|  |_|\____\___/
 *                           research group
 *                             dramco.be/
 *
 *  KU Leuven - Technology Campus Gent,
 *  Gebroeders De Smetstraat 1,
 *  B-9000 Gent, Belgium
 *
 *         File: lpp.h
 *      Created: 2018-03-23
 *       Author: Geoffrey Ottoy
 *
 *  Description: Header file for lpp.c
 */


#ifndef _LPP_H_
#define _LPP_H_

#include <em_device.h>
#include <stdbool.h>

typedef struct lpp_buffer{
	uint8_t * buffer;
	uint8_t fill;
	uint8_t length;
} LPP_Buffer_t;

bool LPP_InitBuffer(LPP_Buffer_t * b, uint8_t size);
void LPP_ClearBuffer(LPP_Buffer_t * b);

bool LPP_AddDigital(LPP_Buffer_t *b, uint8_t data);
bool LPP_AddAnalog(LPP_Buffer_t *b, int16_t data);
bool LPP_AddTreeTemperature(LPP_Buffer_t *b, int16_t data);
bool LPP_AddAmbientTemperature(LPP_Buffer_t *b, int16_t data);
bool LPP_AddAirHumidity(LPP_Buffer_t *b, uint8_t data);
bool LPP_AddSoilMoisture(LPP_Buffer_t *b, uint8_t data);
bool LPP_AddAccelerometer(LPP_Buffer_t *b, int16_t x, int16_t y, int16_t z);
bool LPP_AddCo2(LPP_Buffer_t *b, uint16_t data);
bool LPP_AddTilt(LPP_Buffer_t *b, uint8_t data);

bool LPP_AddConcentration(LPP_Buffer_t *b, uint32_t data);

#endif /* _LPP_H_ */
