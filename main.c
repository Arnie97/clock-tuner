/* A simple user interface for this project

Copyright (C) 2016 Arnie97

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include <stdint.h>
#include <hpsys.h>
#include <hpstdio.h>
#include <hpconio.h>
#include <hpgraphics.h>
#include "hp39kbd.h"
#include "main.h"


int
event_handler(unsigned row, unsigned col)
{
	// [APLET]
	if (row == 0 && col == 7) {
		// exit immediately
		return 5;
	} else {
		// wait until the key is released
		while (any_key_pressed);
	}

	// [UP]: 0, [LEFT]: 1, [DOWN]: 2, [RIGHT]: 3
	if (col == 6 && row < 4) {
		return row + 20;
	} else if (row <= 6 && col <= 4) {
		int ch = row * 5 - col + 'D';  // letter keys
		if (ch == 'D') {
			return 0;  // [DEL]
		} else if (ch < 'D') {
			ch++;  // skip the [DEL] key after [D]
		} else if (ch >= 'T') {
			ch--;  // skip the [ALPHA] key before [T]
			if (ch >= 'X') {
				ch--;  // skip the [SHIFT] key before [X]
				if (row == 6 && col == 0) {
					return 6;  // [ENTER]
				} else if (ch > 'Z') {
					return 0;
				}
			}
		}
		return ch;
	}

	// unhandled keys
	return 0;
}


int
hex_viewer(void *address)
{
	clear_screen();
	printf(
		"[0x%02x]  0 1 2 3  4 5 6 7 01234567",
		(unsigned)address >> 24
	);

	char *p = address;
	for (int row = 1; row < 10; row++) {
		printf(
			"%06x %08x %08x ",
			(unsigned)p & 0x00FFFFFF,
			*p, *(p + 4)
		);
		for (int bit = 0; bit < 8; bit++, p++) {
			#define isprint(c) ((c) >= ' ')
			putchar(isprint(*p)? *p: '.');
		}
	}

	for (;;) {
		int key = get_key();
		if (key == 5) {
			return 0;  // exit program
		} else if (key == 20) {
			return hex_viewer(address - 0x40);
		} else if (key == 21) {
			return hex_viewer(address - 0x10000);
		} else if (key == 22) {
			return hex_viewer(address + 0x40);
		} else if (key == 23) {
			return hex_viewer(address + 0x10000);
		}
	}
}
