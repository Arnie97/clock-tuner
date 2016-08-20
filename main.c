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
		return 25;
	} else {
		// wait until the key is released
		while (any_key_pressed);
	}

	// [UP]: 0, [LEFT]: 1, [DOWN]: 2, [RIGHT]: 3
	if (col == 6 && row < 4) {
		return row + 20;
	} else if (3 <= row && row <= 5 && 1 <= col && col <= 3) {
		return (6 - row) * 3 - col + 1;
	}

	// unhandled keys
	return 0;
}

inline int
sat_strlen(unsigned sat_addr)
{
	return ((int)sat_peek_sat_addr(sat_addr + 5) - 5) / 2;
}


inline char *
sat_strdup(unsigned sat_addr)
{
	unsigned len = sat_strlen(sat_addr);
	char *buf = sys_chkptr(malloc(len + 1));
	buf[len] = '\0';
	return sat_peek_sat_bytes(buf, sat_addr + 10, len);
}


void
display_title(const char *str)
{
	unsigned len = strlen(str) + 2;
	extern int __scr_w;
	int right = (__scr_w - len) / 2, left = __scr_w - len - right;

	clear_screen();
	while (left--) {
		putchar('\x7f');
	}
	putchar(' ');
	while (*str) {
		putchar(*str++);
	}
	putchar(' ');
	while (right--) {
		putchar('\x7f');
	}
	for (int i = 0; i < 6; i++) {
		hpg_set_indicator(i, 0x00);
	}
}


int
note_explorer(void)
{
	unsigned count = 0;
	display_title("Neko Notepad");
	putchar('\n');

	SAT_DIR_NODE *dir = _sat_find_path("/'notesdir");
	for (SAT_DIR_ENTRY *entry = dir->object; entry; entry = entry->next) {
		SAT_OBJ_DSCR *obj = entry->sat_obj;
		if (obj->name[0] == ';') {
			continue;
		}
		count++;
		printf(
			" %2u %21s%6d\n",
			count, obj->name + 1, sat_strlen(obj->addr)
		);
	}
	gotoxy(0, 9);
	printf("%u note entries", count);

	for (;;) {
		int key = get_key();
		if (key == 25) {
			return 0;  // exit program
		} else if (1 <= key && key <= 9) {
			for (SAT_DIR_ENTRY *entry = dir->object; entry; entry = entry->next) {
				SAT_OBJ_DSCR *obj = entry->sat_obj;
				if (obj->name[0] == ';') {
					continue;
				}
				key--;
				if (!key) {
					return note_viewer(obj, 0);
				}
			}
		}
	}
}


int
note_viewer(SAT_OBJ_DSCR *obj, unsigned offset)
{
	display_title(obj->name + 1);
	char *buf = sat_strdup(obj->addr);
	puts(buf + offset);
	free(buf);
	for (;;) {
		int key = get_key();
		if (key == 25) {
			return 0;  // exit program
		}
	}
}
