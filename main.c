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

#include <hpgcc49.h>
#include <hpgraphics.h>
#include "s3c2410.h"
#include "hp39kbd.h"
#include "mpllcon.h"
#include "clkslow.h"
#include "main.h"


int
event_handler(unsigned row, unsigned col)
{
	// [SYMB], [APLET]
	if (row == 6 && col == 5 || row == 0 && col == 7) {
		// exit immediately
		return -1;
	} else {
		// wait until the key is released
		while (any_key_pressed);
	}

	// [UP]: 0, [DOWN]: 2, [PLOT]: 4, [HOME]: 6
	if (col == 6 && !(row & 1)) {
		return (row >> 1) + 1;
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
				if (ch > 'Z') {
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
show_system_info(void)
{
	clear_screen();
	printf(
		"FREQUENCY TUNER\n\n"
		"Build 20160707 by Arnie97\n\n"
	);
	for (int i = 0; i < 6; i++) {
		delay(500000);
		hpg_set_indicator((i + 1) % 6, 0xFF);
		hpg_set_indicator(i, 0x00);
	}

	int status;
	float fclk = clkslow_to_freq(CLKSLOW, FIN);
	if (fclk) {
		// slow mode
		status = 4;
	} else {
		// normal mode
		fclk = mpllcon_to_freq(MPLLCON, FIN);
		status = is_valid_mpllcon(MPLLCON);
	}
	const char *msg[] = {
		"UNKNOWN", "DEFAULT", "CHANGED", "PROFILE",
		"SLOW MODE", "ENABLED"
	};
	printf(
		"MPLLCON %08x at %08x\n"
		"CLKSLOW %08x at %08x\n"
		"  (%.2f MHz, %s %s)\n"
		"\n",
		*MPLLCON, (unsigned)MPLLCON,
		*CLKSLOW, (unsigned)CLKSLOW,
		fclk, msg[status], msg[status == 4? 5: 3]
	);

	delay(750000);
	hpg_set_indicator(0, 0x00);
	printf(
		"CONFIG: [PLOT]   EXIT:    [APLET]"
		"ABOUT:  [SYMB]   REFRESH: [HOME]"
	);

	for (;;) {
		switch (get_key()) {
		case 3:  // [PLOT]
			return show_freq_config(0);
		case 4:  // [HOME]
			return show_system_info();
		case -1:
			return 0;  // exit program
		}
	}
}


int
show_freq_config(int page)
{
	clear_screen();
	printf(
		"SELECT YOUR DESIRED FREQUENCY:\n\n"
	);

	int beg, end;
	char c = 'A';
	if (page == 0) {
		for (int i = 6; i >= 0; i--) {
			unsigned clkslow = 0x10 | i;
			printf(
				"[%c] %-7.2f",
				c++, clkslow_to_freq(&clkslow, FIN)
			);
		}
		beg = 0, end = 8;
	} else {
		beg = 8, end = 22;
	}
	for (int i = beg; i < end; i++) {
		printf(
			"[%c] %-7.2f",
			c++, mpllcon_to_freq(valid_mpllcon_values + i, FIN)
		);
	}
	gotoxy(0, 8);
	printf(
		"BACK:   [HOME]   EXIT:    [APLET]"
		"ABOUT:  [SYMB]   %s:    [%s]",
		page? "PREV": "NEXT", page? "UP": "DOWN"
	);

	for (;;) {
		int k = get_key();
		if (k == 1 || k == 3)  // [UP], [PLOT]
			return show_freq_config(0);
		else if (k == 2)  // [DOWN]
			return show_freq_config(1);
		else if (k == 4)  // [HOME]
			return show_system_info();
		else if (k == -1)
			return 0;  // exit program
		else if (k == 0)
			continue;
		else  // letter keys
			putchar(k);  // TODO: frequency change confirming page
	}
}
