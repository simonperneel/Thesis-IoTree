/*
 *  author: Simon Perneel
 *  Description: Basic Low Power Payload (LPP) functionality.
 *  information from Cayenne Docs: https://developers.mydevices.com/cayenne/docs/lora/#lora-cayenne-low-power-payload
 */

#include <stdlib.h>
#include <stdbool.h>

#include "lpp.h"

// lpp types (defined by Cayenne)
#define LPP_DIGITAL_INPUT				0x00
#define LPP_DIGITAL_OUTPUT				0x01
#define LPP_ANALOG_INPUT				0x02
#define LPP_PRESENCE					0x66
#define LPP_TEMPERATURE					0x67
#define LPP_HUMIDITY					0x68
#define LPP_ACCELEROMETER				0x71
#define LPP_PRESSURE					0x73
#define LPP_GPSLOCATION					0x88
#define LPP_CONCENTRATION			    0x7D

// lpp data sizes
#define LPP_DIGITAL_INPUT_SIZE			0x03
#define LPP_PRESENCE_SIZE				0x03
#define LPP_ANALOG_INPUT_SIZE			0x02
#define LPP_TEMPERATURE_SIZE			0x04
#define LPP_HUMIDITY_SIZE				0x03
#define LPP_ACCELEROMETER_SIZE			0x08
#define LPP_PRESSURE_SIZE				0x04
#define LPP_GPS_SIZE					0x11
#define LPP_CONCENTRATION_SIZE			0x05


// lpp channel ids
#define LPP_DIGITAL_INPUT_CHANNEL		0x01
#define LPP_ANALOG_INPUT_CHANNEL		0x02
#define LPP_AMBIENT_TEMPERATURE_CHANNEL	0x03
#define LPP_AIRHUMIDITY_CHANNEL			0x04
#define LPP_ACCELEROMETER_CHANNEL		0x05
#define LPP_PRESSURE_CHANNEL			0x06
#define LPP_SOILMOISTURE_CHANNEL		0x07
#define LPP_TREE_TEMPERATURE_CHANNEL	0x08
#define LPP_CO2_CHANNEL					0x09
#define LPP_CONCENTRATION_CHANNEL		0x0A
#define LPP_TILT_CHANNEL				0x0B


bool LPP_InitBuffer(LPP_Buffer_t *b, uint8_t size){
	LPP_ClearBuffer(b);

	b->buffer = (uint8_t *) malloc(sizeof(uint8_t) * size);

	if(b->buffer != NULL){
		b->fill = 0;
		b->length = size;
		return true;
	}

	return false;
}

void LPP_ClearBuffer(LPP_Buffer_t *b){
	if(b->buffer != NULL){
		free(b->buffer);
	}
}

bool LPP_AddAnalog(LPP_Buffer_t *b, int16_t data){
	uint8_t space = b->length - b->fill;
	if(space < LPP_ANALOG_INPUT_SIZE){
		return false;
	}

	b->buffer[b->fill++] = LPP_ANALOG_INPUT_CHANNEL;
	b->buffer[b->fill++] = LPP_ANALOG_INPUT;
	b->buffer[b->fill++] = (uint8_t)((0xFF00 & data) >> 8);
	b->buffer[b->fill++] = (uint8_t)(0x00FF & data);

	return true;
}

bool LPP_AddAmbientTemperature(LPP_Buffer_t *b, int16_t data){
	uint8_t space = b->length - b->fill;
	if(space < LPP_TEMPERATURE_SIZE){
		return false;
	}

	b->buffer[b->fill++] = LPP_AMBIENT_TEMPERATURE_CHANNEL;
	b->buffer[b->fill++] = LPP_TEMPERATURE;
	b->buffer[b->fill++] = (uint8_t)((0xFF00 & data) >> 8);
	b->buffer[b->fill++] = (uint8_t)(0x00FF & data);

	return true;
}

bool LPP_AddTreeTemperature(LPP_Buffer_t *b, int16_t data){
	uint8_t space = b->length - b->fill;
	if(space < LPP_TEMPERATURE_SIZE){
		return false;
	}

	b->buffer[b->fill++] = LPP_TREE_TEMPERATURE_CHANNEL;
	b->buffer[b->fill++] = LPP_TEMPERATURE;
	b->buffer[b->fill++] = (uint8_t)((0xFF00 & data) >> 8);
	b->buffer[b->fill++] = (uint8_t)(0x00FF & data);

	return true;
}

bool LPP_AddAirHumidity(LPP_Buffer_t *b, uint8_t data){
	uint8_t space = b->length - b->fill;
	if(space < LPP_HUMIDITY_SIZE){
		return false;
	}

	b->buffer[b->fill++] = LPP_AIRHUMIDITY_CHANNEL;
	b->buffer[b->fill++] = LPP_HUMIDITY;
	b->buffer[b->fill++] = data;

	return true;
}

bool LPP_AddSoilMoisture(LPP_Buffer_t *b, uint8_t data){
	uint8_t space = b->length - b->fill;
	if(space < LPP_HUMIDITY_SIZE){
		return false;
	}

	b->buffer[b->fill++] = LPP_SOILMOISTURE_CHANNEL;
	b->buffer[b->fill++] = LPP_HUMIDITY;
	b->buffer[b->fill++] = data;

	return true;
}

bool LPP_AddConcentration(LPP_Buffer_t *b, uint32_t data){
	uint32_t space = b->length - b->fill;
	if(space < LPP_CONCENTRATION_SIZE){
		return false;
	}

	b->buffer[b->fill++] = LPP_CONCENTRATION_CHANNEL;
	b->buffer[b->fill++] = LPP_CONCENTRATION;
	b->buffer[b->fill++] = (uint8_t)((0xFFFF00 & data) >> 16);
	b->buffer[b->fill++] = (uint8_t)((0xFF00FF & data) >> 8);
	b->buffer[b->fill++] = (uint8_t)(0x00FFFF & data);

	return true;
}

bool LPP_AddAccelerometer(LPP_Buffer_t *b, int16_t x, int16_t y, int16_t z){
	uint8_t space = b->length - b->fill;
	if(space < LPP_ACCELEROMETER_SIZE){
		return false;
	}

	b->buffer[b->fill++] = LPP_ACCELEROMETER_CHANNEL;
	b->buffer[b->fill++] = LPP_ACCELEROMETER;
	b->buffer[b->fill++] = (uint8_t)((0xFF00 & x) >> 8);
	b->buffer[b->fill++] = (uint8_t)(0x00FF & x);
	b->buffer[b->fill++] = (uint8_t)((0xFF00 & y) >> 8);
	b->buffer[b->fill++] = (uint8_t)(0x00FF & y);
	b->buffer[b->fill++] = (uint8_t)((0xFF00 & z) >> 8);
	b->buffer[b->fill++] = (uint8_t)(0x00FF & z);

	return true;
}

bool LPP_AddCo2(LPP_Buffer_t *b, uint16_t data){  //disguised LPP_AddPressure for sending Co2 values
	uint8_t space = b->length - b->fill;
	if(space < LPP_PRESSURE_SIZE){
		return false;
	}

	b->buffer[b->fill++] = LPP_CO2_CHANNEL;
	b->buffer[b->fill++] = LPP_PRESSURE;
	b->buffer[b->fill++] = (uint8_t)((0xFF00 & data) >> 8);
	b->buffer[b->fill++] = (uint8_t)(0x00FF & data);

	return true;
}
