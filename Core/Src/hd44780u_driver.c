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
	LL_mDelay(110);
	hd44780u_write_nibble(0x3U);
	LL_mDelay(4);
	hd44780u_write_nibble(0x3U);
	LL_mDelay(1);
	hd44780u_write_nibble(0x3U);
	LL_mDelay(1);
	hd44780u_write_nibble(0x2U);
	LL_mDelay(1);

	// DISPLAY NOW IN 4-BIT MODE
	hd44780u_write_command(0x2U); // Real function set: 2 Lines & 5x8 font
	hd44780u_write_command(HD44780U_DISPLAY_CTRL | HD44780U_DISPLAY_OFF);
	LL_mDelay(1);

	hd44780u_write_command(HD44780U_DISPLAY_CTRL | HD44780U_DISPLAY_OFF);
	LL_mDelay(1);

	hd44780u_write_command(HD44780U_DISPLAY_CLEAR);
	LL_mDelay(3);
	// Set address counter to increment after ddram write
	hd44780u_write_command(HD44780U_ENTRY_MODE_SET | HD44780U_ENTRY_MODE_INC);
	LL_mDelay(1);
}


void hd44780u_write_nibble(uint8_t nibble)
{
	if (nibble & 0x1U) {
		HD44780U_PORT->BSRR = (uint32_t)HD44780U_D4_PIN;
	} else {
		HD44780U_PORT->BRR = (uint32_t)HD44780U_D4_PIN;
	}
	if (nibble & 0x2U) {
		HD44780U_PORT->BSRR = (uint32_t)HD44780U_D5_PIN;
	} else {
		HD44780U_PORT->BRR = (uint32_t)HD44780U_D5_PIN;
	}
	if (nibble & 0x4U) {
		HD44780U_PORT->BSRR = (uint32_t)HD44780U_D6_PIN;
	} else {
		HD44780U_PORT->BRR = (uint32_t)HD44780U_D6_PIN;
	}
	if (nibble & 0x8U) {
		HD44780U_PORT->BSRR = (uint32_t)HD44780U_D7_PIN;
	} else {
		HD44780U_PORT->BRR = (uint32_t)HD44780U_D7_PIN;
	}
}

void hd44780u_write_command(uint8_t command)
{
	HD44780U_PORT->BRR = HD44780U_RS_PIN; // RS pin low to select instruction register
	hd44780u_write_nibble(command >> 4U); // Shift upper nibble to lower bits for first write
	HD44780U_PULSE_EN();
	hd44780u_write_nibble(command & 0xFU); // Now we only care about the lower nibble
	HD44780U_PULSE_EN();
}

void hd44780u_write_data(uint8_t addr)
{
	HD44780U_PORT->BSRR = HD44780U_RS_PIN;
	hd44780u_write_nibble(addr >> 4U);
	HD44780U_PULSE_EN();
	hd44780u_write_nibble(addr & 0xFU);
	HD44780U_PULSE_EN();
}

Hd44780u_status hd44780u_display_on(hd44780u* display, uint8_t flags)
{
	// Max flag value comes from ORing the two
	if (flags > (HD44780U_CURSOR_ON | HD44780U_BLINK_ON)) {
		return HD44780U_INVALID_FLAGS;
	}
	display->cursor_pos.col = 0;
	display->cursor_pos.row = 0;
	display->display_on_status = HD44780U_DISPLAY_CTRL | HD44780U_DISPLAY_ON | flags;
	hd44780u_write_command(display->display_on_status);
	return HD44780U_OK;
}

void hd44780u_display_off(hd44780u* display)
{
	display->display_on_status = HD44780U_DISPLAY_CTRL | HD44780U_DISPLAY_OFF;
	hd44780u_write_command(display->display_on_status);
}

void hd44780u_display_clear(hd44780u* display)
{
	display->cursor_pos.col = 0;
	display->cursor_pos.row = 0;
	hd44780u_write_command(HD44780U_DISPLAY_CLEAR);
}

void hd44780u_display_home(hd44780u* display)
{
	display->cursor_pos.col = 0;
	display->cursor_pos.row = 0;
	hd44780u_write_command(HD44780U_RETURN_HOME);
}


Hd44780u_status hd44780u_shift_cursor(hd44780u* display, uint8_t direction)
{
	if (direction != HD44780U_SHIFT_LEFT && direction != HD44780U_SHIFT_RIGHT) {
		return HD44780U_INVALID_FLAGS;
	}

	if ((direction == HD44780U_SHIFT_LEFT && display->cursor_pos.col == HD44780U_MIN_COL_POS)
	|| (direction == HD44780U_SHIFT_RIGHT && display->cursor_pos.col == HD44780U_MAX_COL_POS)) {
		return HD44780U_INVALID_DISPLAY_POS;
	}

	if (direction == HD44780U_SHIFT_LEFT) {
		--display->cursor_pos.col;
	} else {
		++display->cursor_pos.col;
	}

	hd44780u_write_command(HD44780U_SHIFT_CTRL | HD44780U_CURSOR_SHIFT | direction);
	return HD44780U_OK;
}

Hd44780u_status hd44780u_set_cursor(hd44780u* display, uint8_t row, uint8_t col)
{
	if (row > HD44780U_MAX_ROW_POS || col > HD44780U_MAX_COL_POS) {
		return HD44780U_INVALID_DISPLAY_POS;
	}

	display->cursor_pos.row = row;
	display->cursor_pos.col = col;

	if (display->cursor_pos.row == HD44780U_MIN_ROW_POS) {
		hd44780u_write_command(HD44780U_DDRAM_ROW_0 + display->cursor_pos.col);
	} else {
		hd44780u_write_command(HD44780U_DDRAM_ROW_1 + display->cursor_pos.col);
	}
	return HD44780U_OK;
}

Hd44780u_status hd44780u_put_char(hd44780u* display, uint8_t c)
{
	if (display->cursor_pos.col > HD44780U_MAX_COL_POS) {
		return HD44780U_INVALID_DISPLAY_POS;
	}
	
	hd44780u_write_data(c);
	++display->cursor_pos.col;
	return HD44780U_OK;
}


Hd44780u_status hd44780u_put_str(hd44780u* display, char* str, size_t len)
{
	// + 1 to account for 0-based ddram addressing
	if (display->cursor_pos.col + len > HD44780U_MAX_COL_POS + 1) {
		return HD44780U_INVALID_DISPLAY_POS;
	}

	while (*str != '\0') {
		hd44780u_put_char(display, *str);
		++str;
	}
	return HD44780U_OK;
}
