/*
 * hd44780u_driver.h
 *
 *  Created on: Aug 16, 2020
 *      Author: Tom
 */

#ifndef INC_HD44780U_DRIVER_H_
#define INC_HD44780U_DRIVER_H_

#include "main.h"
#include "stdint.h"
#include "assert.h"
#include "stddef.h"
#include "hd44780u_driver_config.h"

// HD44780U INSTRUCTION SET
#define HD44780U_DISPLAY_CLEAR 		(uint8_t)0x1U
#define HD47780U_RETURN_HOME 		(uint8_t)0x2U
#define HD44780U_ENTRY_MODE_SET 	(uint8_t)0x4U
#define HD44780U_DISPLAY_CTRL 		(uint8_t)0x8U
#define HD44780U_SHIFT_CTRL 		(uint8_t)0x10U
#define HD47780U_FUNCTION_SET 		(uint8_t)0x20U
#define HD44780U_SET_CGRAM_ADDR 	(uint8_t)0x40U
#define HD44780U_SET_DDRAM_ADDR 	(uint8_t)0x80U

// *** HD44780U INSTRUCTION FLAGS ***

// HD44780U ENTRY MODE SET FLAGS
#define HD44780U_ENTRY_MODE_INC 	(uint8_t)0x2U
#define HD44780U_ENTRY_MODE_DEC 	(uint8_t)0x0U
#define HD44780U_ENTRY_MODE_SHIFT 	(uint8_t)0x1U

// HD44780U DISPLAY CONTROL FLAGS
#define HD44780U_DISPLAY_ON 		(uint8_t)0x4U
#define HD44780U_DISPLAY_OFF 		(uint8_t)0x0U
#define HD44780U_CURSOR_ON 			(uint8_t)0x2U
#define HD44780U_CURSOR_OFF 		(uint8_t)0x0U
#define HD44780U_BLINK_ON 			(uint8_t)0x1U
#define HD44780U_BLINK_OFF 			(uint8_t)0x0U

// HD44780U SHIFT CONTROL FLAGS
#define HD44780U_CURSOR_SHIFT 		(uint8_t)0x0U
#define HD44780U_DISPLAY_SHIFT 		(uint8_t)0x8U
#define HD44780U_SHIFT_LEFT 		(uint8_t)0x0U
#define HD44780U_SHIFT_RIGHT 		(uint8_t)0x4U

// HD44780U FUNCTION SET FLAGS
#define HD44780U_5x8_CHAR_FONT 		(uint8_t)0x0U
#define HD44780U_5x10_CHAR_FONT 	(uint8_t)0x4U
#define HD44780U_1_DISPLAY_LINES 	(uint8_t)0x0U
#define HD44780U_2_DISPLAY_LINES 	(uint8_t)0x8U
#define HD44780U_4_BIT_INTERFACE 	(uint8_t)0x0U
#define HD44780U_8_BIT_INTERFACE 	(uint8_t)0x10U

#define HD44780U_MIN_DDRAM_ADDR 	(uint8_t)0x0U
#define HD44780U_MAX_DDRAM_ADDR 	(uint8_t)0x4AU

#define HD44780U_PULSE_EN() {\
		HD44780U_PORT->BSRR = HD44780U_EN_PIN; \
		LL_mDelay(1);                          \
		HD44780U_PORT->BRR = HD44780U_EN_PIN;  \
	}

typedef enum {
	HD44780U_OK
} Hd44780u_status;

void hd44780u_init(void);
void hd44780u_write_nibble(uint8_t nibble);
void hd44780u_write_command(uint8_t command);
void hd44780u_write_data(uint8_t addr);
void hd44780u_display_on(uint8_t flags);
void hd44780u_display_off(void);
void hd44780u_display_clear(void);
void hd44780u_cursor_shift(uint8_t direction);
void hd44780u_display_shift(uint8_t direction);
void hd44780u_set_cgram_addr(uint8_t addr);
void hd44780u_set_ddram_addr(uint8_t addr);
void hd44780u_put_char(uint8_t ddram_addr, uint8_t c);
void hd44780u_put_str(const char *str, size_t len, uint8_t start_addr);

#endif /* INC_HD44780U_DRIVER_H_ */
