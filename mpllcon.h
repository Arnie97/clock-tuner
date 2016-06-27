/* S3C2410 MPLLCON manipulating module

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

#ifndef _MPLLCON_H
#define _MPLLCON_H

extern const unsigned valid_mpllcon_values[];

struct mpllcon {
	unsigned sdiv:  2;
	unsigned     :  2;
	unsigned pdiv:  6;
	unsigned     :  2;
	unsigned mdiv:  8;
	unsigned     : 12;
};

float mpllcon_to_freq(struct mpllcon *reg, unsigned fin);
int is_valid_mpllcon(unsigned *reg);

#endif
