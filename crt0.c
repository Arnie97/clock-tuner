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

#include <hpgcc49.h>

#define delay(t) { volatile int i = (t); while (i--); }

extern unsigned int *__exit_stk_state;
extern int _exit_save(unsigned int *ptr);
extern void __exit_cleanup();

// default monochrome display buffer
unsigned __display_buf;


void _start()
{
	// read in the video buffer address
	// it assumes that the MMU mappings haven't changed
	// and the buffer is in the 4kb SRAM
	volatile unsigned *LCDSADDR1 = (unsigned *)0x7300014;
	__display_buf = (*LCDSADDR1 & 0xffff) * 2 + 0x7f00000;

	// set the screen height for the <hpstdio.h> module
	extern int __scr_h;
	__scr_h = (sys_lcdgetheight() == 64)? 10: 13;

	// declare locally to reduce executable size
	unsigned state_buffer[4], lcd_buffer[17], clk_buffer[6];
	__exit_stk_state = state_buffer;

	volatile unsigned
		*const clk_registers = (unsigned *)0x07200004,
		*const lcd_registers = (unsigned *)0x07300000,
		*const rtc_registers = (unsigned *)0x07B00000;

	const unsigned
		clk = *clk_registers,
		lcd = *lcd_registers,
		rtc = *rtc_registers;

	// turn interrupts off so we can use the screen
	sys_intOff();

	// save current hardware state
	sys_lcdsave(lcd_buffer);
	sys_clksave(clk_buffer);

	// slow mode on by default - makes sense in the majority of applications
	sys_slowOn();

	// will return 0 when exitting
	if (_exit_save((unsigned *)state_buffer)) {
		clear_screen();
		printf(
			"SYSTEM INFORMATION\n\n"
			"Build 20160626 by Arnie97\n\n"
			"    %08x    %08x\n"
			"CLK %08x at %08x\n"
			"LCD %08x at %08x\n"
			"RTC %08x at %08x\n"
			"\n",
			0x01234567,
			0x89abcdef,
			clk, (unsigned)clk_registers,
			lcd, (unsigned)lcd_registers,
			rtc, (unsigned)rtc_registers
		);
		printf("PRESS [APLET] KEY TO EXIT");
		while (!keyb_isAnyKeyPressed());
		__exit_cleanup();
	}

	// restore original hardware values
	sys_lcdrestore(lcd_buffer);
	sys_clkrestore(clk_buffer);

	// turn interrupts on for the OS
	sys_intOn();
}
