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

Hd44780u_status hd44780u_display_on(hd44780u* display, uint8_t cursor_flags)
{
	if (cursor_flags > (HD44780U_CURSOR_ON | HD44780U_BLINK_ON)) {
		return HD44780U_INVALID_FLAGS;
	}

	display->cursor.row = 0;
	display->cursor.col = 0;
	display->display_status = HD44780U_DISPLAY_ON | cursor_flags;
	hd44780u_write_command(HD44780U_DISPLAY_CTRL | display->display_status);
	return HD44780U_OK;
}

void hd44780u_display_off(hd44780u* display)
{
	display->display_status = HD44780U_DISPLAY_OFF;
	hd44780u_write_command(HD44780U_DISPLAY_CTRL | display->display_status);
}

void hd44780u_display_clear(hd44780u* display)
{
	display->cursor.row = 0;
	display->cursor.col = 0;
	hd44780u_write_command(HD44780U_DISPLAY_CLEAR);
}

void hd44780u_display_home(hd44780u* display)
{
	display->cursor.row = 0;
	display->cursor.col = 0;
	hd44780u_write_command(HD44780U_RETURN_HOME);
}

void hd44780u_cursor_on(hd44780u* display)
{
	display->display_status = HD44780U_DISPLAY_ON | HD44780U_CURSOR_ON;
	hd44780u_write_command(HD44780U_DISPLAY_CTRL | display->display_status);
}

void hd44780u_cursor_off(hd44780u* display)
{
	display->display_status = HD44780U_DISPLAY_ON | HD44780U_CURSOR_OFF;
	hd44780u_write_command(HD44780U_DISPLAY_CTRL | display->display_status);
}

void hd44780u_blink_on(hd44780u* display)
{
	display->display_status = HD44780U_DISPLAY_ON | HD44780U_CURSOR_ON | HD44780U_BLINK_ON;
	hd44780u_write_command(HD44780U_DISPLAY_CTRL | display->display_status);
}

void hd44780u_blink_off(hd44780u* display)
{
	display->display_status = HD44780U_DISPLAY_ON | HD44780U_CURSOR_ON | HD44780U_BLINK_OFF;
	hd44780u_write_command(HD44780U_DISPLAY_CTRL | display->display_status);
}

Hd44780u_status hd44780u_shift_cursor(hd44780u* display, uint8_t direction)
{
	if (direction != HD44780U_SHIFT_LEFT && direction != HD44780U_SHIFT_RIGHT) {
		return HD44780U_INVALID_FLAGS;
	}

	if ((direction == HD44780U_SHIFT_LEFT && display->cursor.col == 0U)
	|| (direction == HD44780U_SHIFT_RIGHT && display->cursor.col == HD44780U_MAX_COL_POS)) {
		return HD44780U_INVALID_DISPLAY_POS;
	}

	if (direction == HD44780U_SHIFT_LEFT) {
		--display->cursor.col;
	} else {
		++display->cursor.col;
	}

	hd44780u_write_command(HD44780U_SHIFT_CTRL | HD44780U_CURSOR_SHIFT | direction);
	return HD44780U_OK;
}

Hd44780u_status hd44780u_set_cursor(hd44780u* display, uint8_t row, uint8_t col)
{
	if (row > HD44780U_MAX_ROW_POS || col > HD44780U_MAX_COL_POS) {
		return HD44780U_INVALID_DISPLAY_POS;
	}

	display->cursor.row = row;
	display->cursor.col = col;

	if (display->cursor.row == 0) {
		hd44780u_write_command(HD44780U_SET_DDRAM_ADDR | display->cursor.col);
	} else {
		//0x40U == DDRAM row 1 offset
		hd44780u_write_command(HD44780U_SET_DDRAM_ADDR | (display->cursor.col + 0x40U));
	}
	return HD44780U_OK;
}

Hd44780u_status hd44780u_put_char(hd44780u* display, uint8_t c)
{
	if (display->cursor.col > HD44780U_MAX_COL_POS) {
		return HD44780U_INVALID_DISPLAY_POS;
	}
	
	hd44780u_write_data(c);
	++display->cursor.col;
	return HD44780U_OK;
}

Hd44780u_status hd44780u_put_str(hd44780u* display, char* str, size_t len)
{
	// + 1 to account for 0-based ddram addressing
	if (display->cursor.col + len > HD44780U_MAX_COL_POS + 1U) {
		return HD44780U_INVALID_DISPLAY_POS;
	}

	while (*str != '\0') {
		hd44780u_put_char(display, *str);
		++str;
	}
	return HD44780U_OK;
}