/*
 * hd44780u_driver.h
 *
 *  Created on: Aug 16, 2020
 *      Author: Tom
 */

#ifndef INC_HD44780U_DRIVER_H_
#define INC_HD44780U_DRIVER_H_

#include "main.h"
#include "hd44780u_driver_config.h"
#include "stdint.h"

// HD44780U INSTRUCTION SET
#define HD44780U_DISPLAY_CLEAR 		0x1U
#define HD47780U_RETURN_HOME 		0x2U
#define HD44780U_ENTRY_MODE_SET 	0x4U
#define HD44780U_DISPLAY_CTRL 		0x8U
#define HD44780U_SHIFT_CTRL 		0x10U
#define HD47780U_FUNCTION_SET 		0x20U
#define HD44780U_SET_CGRAM_ADDR 	0x40U
#define HD44780U_SET_DDRAM_ADRR 	0x80U

// *** HD44780U INSTRUCTION FLAGS ***

// HD44780U ENTRY MODE SET FLAGS
#define HD44780U_ENTRY_MODE_INCREMENT 	0x2U
#define HD44780U_ENTRY_MODE_DECREMENT 	0x0U
#define HD44780U_ENTRY_MODE_SHIFT 		0x1U

// HD44780U DISPLAY CONTROL FLAGS
#define HD44780U_DISPLAY_ON 	0x4U
#define HD44780U_DISPLAY_OFF 	0x0U
#define HD44780U_CURSOR_ON 		0x2U
#define HD44780U_CURSOR_OFF 	0x0U
#define HD44780U_BLINK_ON 		0x1U
#define HD44780U_BLINK_OFF 		0x0U

// HD44780U SHIFT CONTROL FLAGS
#define HD44780U_CURSOR_SHIFT 	0x0U
#define HD44780U_DISPLAY_SHIFT 	0x8U
#define HD44780U_SHIFT_LEFT 	0x0U
#define HD44780U_SHIFT_RIGHT 	0x4U

#define HD44780U_PULSE_EN() {\
		HD44780U_PORT->BSRR = HD44780U_EN_PIN; \
		LL_mDelay(1);                          \
		HD44780U_PORT->BRR = HD44780U_EN_PIN;  \
	}

void hd44780u_init(void);
void hd44780u_write_nibble(uint8_t nibble);
void hd44780u_write_command(uint8_t command);
void hd44780u_display_on(uint8_t flags);
void hd44780u_display_off(void);
void hd44780u_cursor_shift(uint8_t direction);
void hd44780u_display_shift(uint8_t direction);

#endif /* INC_HD44780U_DRIVER_H_ */
