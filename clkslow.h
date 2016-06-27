/* S3C2410 CLKSLOW manipulating module

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

#ifndef _CLKSLOW_H
#define _CLKSLOW_H

struct clkslow {
	unsigned slow_val:  3;
	unsigned         :  1;
	unsigned slow_bit:  1;
	unsigned mpll_off:  1;
	unsigned         :  1;
	unsigned uclk_on :  1;
};

float clkslow_to_freq(struct clkslow *reg, unsigned fin);

#endif
