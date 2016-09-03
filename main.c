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

#include <hpstdio.h>
#include <hpconio.h>
#include "s3c2410.h"
#include "hp39kbd.h"
#include "display.h"
#include "mpllcon.h"
#include "clkslow.h"
#include "main.h"

const char title[] = "Clock Tuner for HP 39g+";


int
event_handler(unsigned row, unsigned col)
{
	// [APLET]
	if (row == 0 && col == 7) {
		// exit immediately
		return 5;
	} else {
		// wait until the key is released
		set_indicator(indicator_wait, TRUE);
		while (any_key_pressed);
		set_indicator(indicator_wait, FALSE);
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
				if (row == 6 && col == 0) {
					return 6;  // [ENTER]
				} else if (ch > 'Z') {
					return 0;
				}
			}
		}
		return ch;
	} else if (row == 6 && col == 5) {
		return 7;  // [SYMB]
	}

	// unhandled keys
	return 0;
}


int
show_system_info(void)
{
	clear_screen();
	printf(
		"%s\n\n"
		"v1.1 build 20160711 by Arnie97\n\n",
		title
	);
	delay(50000);
	for (int i = 0; i < 6; i++) {
		set_indicator((i + 1) % 6, TRUE);
		set_indicator(i, FALSE);
		delay(400000);
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
		"  (%.2f MHz, %s %s)\n",
		*MPLLCON, (unsigned)MPLLCON,
		*CLKSLOW, (unsigned)CLKSLOW,
		fclk, msg[status], msg[status == 4? 5: 3]
	);

	delay(500000);
	set_indicator(0, FALSE);
	gotoxy(0, 8);
	printf(
		"Config:  [PLOT]   Exit:   [APLET]"
		"Refresh: [HOME]   About:  [SYMB]"
	);

	for (;;) {
		int k = get_key();
		if (k == 3)  // [PLOT]
			return show_freq_config(0);
		else if (k == 4)  // [HOME]
			return show_system_info();
		else if (k == 5)  // [APLET]
			return 0;  // exit program
		else if (k == 7)  // [SYMB]
			return show_credits();
	}
}


int
show_freq_config(int page)
{
	clear_screen();
	printf(
		"Select your desired frequency:\n\n"
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
		beg = 8, end = 23;
	}
	for (int i = beg; i < end; i++) {
		printf(
			"[%c] %-7.2f",
			c++, mpllcon_to_freq(valid_mpllcon_values + i, FIN)
		);
	}
	gotoxy(0, 8);
	printf(
		"Back:    [HOME]   Exit:   [APLET]"
		"About:   [SYMB]   %s:   [%s]",
		page? "Prev": "Next", page? "UP": "DOWN"
	);

	for (;;) {
		int k = get_key();
		if (k == 0)  // unknown keys
			continue;
		else if (k == 1 || k == 3)  // [UP], [PLOT]
			return show_freq_config(0);
		else if (k == 2)  // [DOWN]
			return show_freq_config(1);
		else if (k == 4)  // [HOME]
			return show_system_info();
		else if (k == 5)  // [APLET]
			return 0;  // exit program
		else if (k == 7)  // [SYMB]
			return show_credits();
		else if ('A' <= k && k < c)  // letter keys
			return show_freq_confirm(end - c + k);
	}
}


int
show_freq_confirm(int choice)
{
	clear_screen();

	// get register config for the selected frequency
	unsigned mpllcon = valid_mpllcon_values[choice < 0? 7: choice];
	unsigned clkslow = choice < 0? 0x10 | (-choice - 1): 0x4;

	// calculate human readable frequencies
	float fclk1 = clkslow_to_freq(CLKSLOW, FIN);
	if (!fclk1) {
		fclk1 = mpllcon_to_freq(MPLLCON, FIN);
	}
	float fclk2 = clkslow_to_freq(&clkslow, FIN);
	if (!fclk2) {
		fclk2 = mpllcon_to_freq(&mpllcon, FIN);
	}

	printf(
		"WARNING!\n"
		"THIS OPERATION MAY DAMAGE YOUR\n"
		"DEVICE. USE AT YOUR OWN RISK.\n"
		"\n"
		"MPLLCON: %08x -> %08x\n"
		"CLKSLOW: %08x -> %08x\n"
		"(Clock: %.2f Mhz -> %.2f MHz)\n",
		*MPLLCON, mpllcon, *CLKSLOW, clkslow,
		fclk1, fclk2
	);
	gotoxy(0, 8);
	printf(
		"Back:    [HOME]   Exit:   [APLET]"
		"OK:      [ENTER]  Cancel: [PLOT]"
	);

	for (;;) {
		int k = get_key();
		if (k == 3)  // [PLOT]
			return show_freq_config(0);
		else if (k == 4)  // [HOME]
			return show_system_info();
		else if (k == 5)  // [APLET]
			return 0;  // exit program
		else if (k == 6)  // [ENTER]
			return show_freq_change(mpllcon, clkslow);
	}
}


int
show_freq_change(unsigned mpllcon, unsigned clkslow)
{
	clear_screen();
	printf("Working hard, please hold on...\n\n");
	sys_LCDSynch();
	*MPLLCON = mpllcon;
	*CLKSLOW = clkslow;
	sys_lcdfix();
	delay(100000);
	printf("Well done.");
	gotoxy(0, 8);
	printf(
		"Back:    [HOME]   Exit:   [APLET]"
		"Config:  [PLOT]   About:  [SYMB]"
	);
	for (;;) {
		int k = get_key();
		if (k == 3)  // [PLOT]
			return show_freq_config(0);
		else if (k == 4)  // [HOME]
			return show_system_info();
		else if (k == 5)  // [APLET]
			return 0;  // exit program
		else if (k == 7)  // [SYMB]
			return show_credits();
	}
}


int
show_credits(void)
{
	clear_screen();
	printf(
		"%s\n\n"
		"Copyright (C) 2005 HP-GCC Team\n"
		"Copyright (C) 2016 Arnie97\n"
		"\n"
		"Clock Tuner is free software; it "
		"comes with absolutely no warranty"
		"See file 'LICENSE' for details.",
		title
	);
	gotoxy(0, 9);
	printf(
		"Back:   [HOME]    Config: [PLOT]"
	);
	for (;;) {
		int k = get_key();
		if (k == 3)  // [PLOT]
			return show_freq_config(0);
		else if (k == 4)  // [HOME]
			return show_system_info();
		else if (k == 5)  // [APLET]
			return 0;  // exit program
	}
}
