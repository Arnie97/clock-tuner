/* S3C2410 register manipulating module

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

#ifndef _S3C2410_H
#define _S3C2410_H

#define delay(t) { volatile int i = (t); while (i--); }

struct mpllcon {
	unsigned sdiv:  2;
	unsigned     :  2;
	unsigned pdiv:  6;
	unsigned     :  2;
	unsigned mdiv:  8;
	unsigned     : 12;
};

struct clkslow {
	unsigned slow_val:  3;
	unsigned         :  1;
	unsigned slow_bit:  1;
	unsigned mpll_off:  1;
	unsigned         :  1;
	unsigned uclk_on :  1;
};

// remapped addresses of registers
#define MPLLCON   ((unsigned *)0x07200004)
#define CLKSLOW   ((unsigned *)0x07200010)
#define CLKDIVN   ((unsigned *)0x07200014)

#define BANKCON0  ((unsigned *)0x07000004)
#define BANKCON1  ((unsigned *)0x07000008)

#define GPFDAT    ((unsigned *)0x07A00054)
#define GPGCON    ((unsigned *)0x07A00060)
#define GPGDAT    ((unsigned *)0x07A00064)

// frequency of external oscillator
#define FIN 12

extern const unsigned valid_mpllcon_values[];
float mpllcon_to_freq(struct mpllcon *reg, unsigned fin);
int is_valid_mpllcon(unsigned *reg);
float clkslow_to_freq(struct clkslow *reg, unsigned fin);

#endif
