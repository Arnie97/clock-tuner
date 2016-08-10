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
#include <saturn.h>
#include <satdir.h>
#include <hpsys.h>
#include <hpstdio.h>
#include <hpconio.h>
#include <hpstring.h>
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
note_explorer(void)
{
	SAT_DIR_NODE *dir = _sat_find_path("/'notesdir");
	SAT_DIR_ENTRY *entry = dir->object;

	clear_screen();
	int count = 0;
	while (entry) {
		SAT_OBJ_DSCR *obj = entry->sat_obj;

		// saturn data are nibble-aligned
		// _saturn_cpu->Read[sat_addr >> 12] + (sat_addr & 0xfff) / 2
		printf("0x%08x %s\n", sat_map_s2a(obj->addr), obj->name);
		entry = entry->next;
		count++;
	}
	printf("\n%d notes in %d entries", count / 2, count);

	for (;;) {
		int key = get_key();
		if (key == 5) {
			return 0;  // exit program
		}
	}
}
