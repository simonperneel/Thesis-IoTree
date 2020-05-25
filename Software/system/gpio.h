/*
 * gpio.h
 *
 *  Created on: 2-okt.-2019
 *      Author: arned
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include <stdbool.h>

void powerSensor(bool enable, GPIO_Port_TypeDef port, unsigned int pin);
void init_GPIO();

#endif /* INC_GPIO_H_ */
