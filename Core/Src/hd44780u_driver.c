/*
 * hd44780u_driver.c
 *
 *  Created on: Aug 16, 2020
 *      Author: Tom
 */


#include "hd44780u_driver.h"

void hd44780u_init(void)
{
	// 8 Bit-mode function set instructions
	LL_mDelay(30);
	hd44780u_write_nibble(0x3);
	LL_mDelay(5);
	hd44780u_write_nibble(0x3);
	LL_mDelay(1);
	hd44780u_write_nibble(0x3);
	LL_mDelay(1);
	hd44780u_write_nibble(0x2);
	LL_mDelay(1);

	// DISPLAY NOW IN 4-BIT MODE
	hd44780u_write_command(0x2); // Real function set: 2 Lines & 5x7 font
	hd44780u_write_command(HD44780U_DISPLAY_CTRL | HD44780U_DISPLAY_OFF);
	LL_mDelay(1);

	hd44780u_write_command(HD44780U_DISPLAY_CTRL | HD44780U_DISPLAY_OFF);
	LL_mDelay(1);

	hd44780u_write_command(HD44780U_DISPLAY_CLEAR);
	LL_mDelay(3);

	hd44780u_write_command(HD44780U_ENTRY_MODE_SET | HD44780U_ENTRY_MODE_INCREMENT);
	LL_mDelay(1);
}


void hd44780u_write_nibble(uint8_t nibble)
{
	if (nibble & 0x1) {
		HD44780U_PORT->BSRR = (uint32_t)HD44780U_D4_PIN;
	} else {
		HD44780U_PORT->BRR = (uint32_t)HD44780U_D4_PIN;
	}
	if (nibble & 0x2) {
		HD44780U_PORT->BSRR = (uint32_t)HD44780U_D5_PIN;
	} else {
		HD44780U_PORT->BRR = (uint32_t)HD44780U_D5_PIN;
	}
	if (nibble & 0x4) {
		HD44780U_PORT->BSRR = (uint32_t)HD44780U_D6_PIN;
	} else {
		HD44780U_PORT->BRR = (uint32_t)HD44780U_D6_PIN;
	}
	if (nibble & 0x8) {
		HD44780U_PORT->BSRR = (uint32_t)HD44780U_D7_PIN;
	} else {
		HD44780U_PORT->BRR = (uint32_t)HD44780U_D7_PIN;
	}
}

void hd44780u_write_command(uint8_t command)
{
	hd44780u_write_nibble(command >> 4); // Shift upper nibble to lower bits for first write
	HD44780U_PULSE_EN();
	hd44780u_write_nibble(command & 0xF); // Now we only care about the lower nibble
	HD44780U_PULSE_EN();
}

void hd44780u_display_on(uint8_t flags)
{
	hd44780u_write_command(HD44780U_DISPLAY_CTRL | HD44780U_DISPLAY_ON | flags);
}

void hd44780u_display_off(void)
{
	hd44780u_write_command(HD44780U_DISPLAY_CTRL | HD44780U_DISPLAY_OFF);
}

void hd44780u_cursor_shift(uint8_t direction)
{
	hd44780u_write_command(HD44780U_SHIFT_CTRL | HD44780U_CURSOR_SHIFT | direction);
}

void hd44780u_display_shift(uint8_t direction)
{
	hd44780u_write_command(HD44780U_SHIFT_CTRL | HD44780U_DISPLAY_SHIFT | direction);
}

