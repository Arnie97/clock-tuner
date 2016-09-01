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

#include <saturn.h>
#include <hpstdio.h>
#include <hpconio.h>
#include <hpstring.h>
#include "hp39kbd.h"
#include "stack.h"
#include "display.h"
#include "main.h"


int
event_handler(unsigned row, unsigned col)
{
	// [APLET], [HOME]
	if (row == 0 && col == 7 || row == 6 && col == 6) {
		// exit immediately
		return 27;
	} else {
		// wait until the key is released
		set_indicator(indicator_wait, TRUE);
		while (any_key_pressed);
		set_indicator(indicator_wait, FALSE);
	}

	// [UP]: 0, [LEFT]: 1, [DOWN]: 2, [RIGHT]: 3
	if (col == 6) {
		return row + 20;
	} else if (row == 1 && col == 7) {
		return 28;  // [VIEWS]
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
		set_indicator(i, FALSE);
	}
}


void
display_item(unsigned count, SAT_OBJ_DSCR *obj)
{
	putchar(' ');
	putchar(' ');
	putchar('0' + count);
	putchar(' ');

	char *name = obj->name + 1;
	while (*name) {
		putchar(*name++);
	}

	char buf[7];
	utoa(sat_strlen(obj->addr), buf, 10);
	for (unsigned i = obj->name - name - strlen(buf) + 28; i > 0; i--) {
		putchar(' ');
	}
	puts(buf);
}


int
note_explorer(SAT_DIR_ENTRY *init)
{
	display_title("Neko Notepad");
	putchar('\n');

	if (!init) {
		SAT_DIR_NODE *dir = _sat_find_path("/'notesdir");
		init = dir->object;
	} else {
		set_indicator(indicator_lshift, TRUE);
	}

	unsigned count = 0;
	SAT_DIR_ENTRY *next_page = NULL;
	for (SAT_DIR_ENTRY *entry = init; entry; entry = entry->next) {
		SAT_OBJ_DSCR *obj = entry->sat_obj;
		if (obj->name[0] == ';') {
			continue;
		}
		if (count == 8) {
			next_page = entry;
			set_indicator(indicator_rshift, TRUE);
			break;
		}
		count++;
		display_item(count, obj);
	}
	gotoxy(0, 9);

	for (;;) {
		int key = get_key();
		if (key == 27) {
			return 0;  // exit program
		} else if ((key == 22 || key == 23) && next_page) {
			return note_explorer(next_page);  // page down
		} else if (key == 20 || key == 21) {
			return note_explorer(NULL);  // first page
		} else if (1 <= key && key <= count) {
			for (SAT_DIR_ENTRY *entry = init; entry; entry = entry->next) {
				SAT_OBJ_DSCR *obj = entry->sat_obj;
				if (obj->name[0] == ';') {
					continue;
				}
				key--;
				if (!key) {
					return note_viewer(obj);
				}
			}
		}
	}
}


int
note_viewer(SAT_OBJ_DSCR *obj)
{
	NODE *head = NULL;
	const char *buf = sat_strdup(obj->addr);
	const char *next_page = buf;
	goto refresh;

	for (;;) {
		int key = get_key();
		if (key == 27) {
			return 0;  // exit program
		} else if ((key == 22 || key == 23) && *next_page) {
			refresh: push(&head, next_page);
			next_page = bitmap_blit(next_page);  // page down
			set_indicator(indicator_lshift, head->data != buf);
		} else if (key == 20 || key == 21) {
			pop(&head);
			next_page = pop(&head);  // page up
			if (!next_page) {
				next_page = buf;
			}
			goto refresh;
		} else if (key == 28) {
			free((void *)buf);
			while (head) {
				pop(&head);
			}
			return note_explorer(NULL);  // go back to the list
		}
	}
}
