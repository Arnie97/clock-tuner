/* Derived from /hpgcc/sources/crt0/crt0.c of HP-GCC Version 1.1

Copyright (C) 2005 The HP-GCC Development Team
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

#include <hpsys.h>
#include "main.h"

extern unsigned int *__exit_stk_state;
extern int _exit_save(unsigned int *ptr);
extern void __exit_cleanup();

// default monochrome display buffer
unsigned __display_buf;


void
_start(void)
{
	// read in the video buffer address
	// it assumes that the MMU mappings haven't changed
	// and the buffer is in the 4kb SRAM
	volatile unsigned *LCDSADDR1 = (unsigned *)0x7300014;
	__display_buf = (*LCDSADDR1 & 0xffff) * 2 + 0x7f00000;

	// set the screen height for the <hpstdio.h> module
	extern int __scr_h;
	__scr_h = 10;

	// declare locally to reduce executable size
	unsigned state_buffer[4], lcd_buffer[17];
	__exit_stk_state = state_buffer;

	// turn interrupts off so we can use the screen
	sys_intOff();

	// save current hardware state
	sys_lcdsave(lcd_buffer);

	// will return 0 when exitting
	if (_exit_save((unsigned *)state_buffer)) {
		note_explorer();
		__exit_cleanup();
	}

	// restore original hardware values
	sys_lcdrestore(lcd_buffer);

	// turn interrupts on for the OS
	sys_intOn();
}
