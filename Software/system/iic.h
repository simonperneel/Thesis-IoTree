/*
 * @file iic.c
 * @brief header file for iic.c
 * @author Simon Perneel
 */

#ifndef SYSTEM_IIC_H_
#define SYSTEM_IIC_H_

#include <stdbool.h>

void IIC_Init(void);
void setupIIC(void);
void IIC_Reset(void);
bool IIC_WriteBuffer(uint8_t iicAddress, uint8_t * wBuffer, uint8_t wLength);
bool IIC_ReadBuffer(uint8_t iicAddress, uint8_t * rBuffer, uint8_t rLength);
bool IIC_WriteReadBuffer(uint8_t iicAddress, uint8_t * wBuffer, uint8_t wLength, uint8_t *rBuffer, uint8_t rLength);

#endif /* SYSTEM_IIC_H_ */
