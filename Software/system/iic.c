/*
 * @file iic.c
 * @brief functions for I²C communication
 * @author Simon Perneel
 */


#include <i2cspm.h>
#include <stdbool.h>
#include <stdint.h>
#include "em_i2c.h"
#include "i2cspm.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "CCS811.h"
#include "pin_mapping.h"
#include "iic.h"

I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;

void IIC_Init(void){
	I2CSPM_Init(&i2cInit);
}

void setupIIC(void){
	// settings i2c
	i2cInit.port = I2C0;
	i2cInit.sclPort = SCL_PORT;
	i2cInit.sclPin = SCL_PIN;
	i2cInit.sdaPort = SDA_PORT;
	i2cInit.sdaPin = SDA_PIN;
	i2cInit.i2cClhr = i2cClockHLRStandard;
	i2cInit.i2cRefFreq = 0 ;
	i2cInit.i2cMaxFreq = 50000; //lowerd standard frequency - I2C_FREQ_STANDARD_MAX;
	i2cInit.portLocation = PORT_LOC; //to be found in datasheet of controller

	I2CSPM_Init(&i2cInit);
}

void IIC_Reset(void){
	I2C_Reset(i2cInit.port);
}

bool IIC_WriteBuffer(uint8_t iicAddress, uint8_t * wBuffer, uint8_t wLength){
	I2C_TransferSeq_TypeDef seq;
	I2C_TransferReturn_TypeDef ret;
	uint8_t i2c_read_data[0];

	seq.addr  = iicAddress;
	seq.flags = I2C_FLAG_WRITE;
	/* Point to write buffer (contains command & data) */
	seq.buf[0].data   = wBuffer;
	seq.buf[0].len    = wLength;
	/* Select location/length of data to be read */
	seq.buf[1].data = i2c_read_data;
	seq.buf[1].len  = 0;

	ret = I2CSPM_Transfer(i2cInit.port, &seq);

	if (ret != i2cTransferDone) {
		return false;
	}

	return true;
}

bool IIC_ReadBuffer(uint8_t iicAddress, uint8_t * rBuffer, uint8_t rLength){
	I2C_TransferSeq_TypeDef seq;
	I2C_TransferReturn_TypeDef ret;

	seq.addr  = iicAddress;
	seq.flags = I2C_FLAG_READ;

	/* Select location/length of data to be read */
	seq.buf[0].data = rBuffer;
	seq.buf[0].len  = rLength;

	ret = I2CSPM_Transfer(i2cInit.port, &seq);

	if (ret != i2cTransferDone) {
		*rBuffer = 0;
		return false;
	}
	return true;
}

bool IIC_WriteReadBuffer(uint8_t iicAddress, uint8_t * wBuffer, uint8_t wLength, uint8_t *rBuffer, uint8_t rLength){
	I2C_TransferSeq_TypeDef seq;
	I2C_TransferReturn_TypeDef ret;

	seq.addr  = iicAddress;
	seq.flags = I2C_FLAG_WRITE_READ;
	/* Point to write buffer (contains command & data) */
	seq.buf[0].data   = wBuffer;
	seq.buf[0].len    = wLength;

	/* Select location/length of data to be read */
	seq.buf[1].data = rBuffer;
	seq.buf[1].len  = rLength;

	ret = I2CSPM_Transfer(i2cInit.port, &seq);

	if (ret != i2cTransferDone) {
		*rBuffer = 0;
		return false;
	}

	return true;
}
