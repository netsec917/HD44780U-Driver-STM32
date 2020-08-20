/*
 * hd44780u_driver_config.h
 *
 *  Created on: Aug 16, 2020
 *      Author: Tom
 */

#ifndef INC_HD44780U_DRIVER_CONFIG_H_
#define INC_HD44780U_DRIVER_CONFIG_H_

#include "main.h"

// Port & Pin constants for interface

#define HD44780U_PORT       GPIOB
#define HD44780U_RS_PIN     LL_GPIO_PIN_5
#define HD44780U_EN_PIN     LL_GPIO_PIN_4
#define HD44780U_D4_PIN     LL_GPIO_PIN_0
#define HD44780U_D5_PIN     LL_GPIO_PIN_7
#define HD44780U_D6_PIN     LL_GPIO_PIN_6
#define HD44780U_D7_PIN     LL_GPIO_PIN_1

#endif /* INC_HD44780U_DRIVER_CONFIG_H_ */
