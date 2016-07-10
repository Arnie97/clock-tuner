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

#include "mpllcon.h"

const unsigned valid_mpllcon_values[] = {
	// hp default
	0x43012,

	// from PLL value selection table in S3C2410 datasheet
	0x52023, 0x52013, 0xa1033, 0x78023, 0x8e023, 0x52022,
	0x47012, 0x69022, 0x70022, 0x7f022, 0x69012, 0x96022,
	0x74012, 0x52021, 0x5a021, 0x44011, 0x47011, 0x4d011,
	0x52011, 0x55011, 0x58011, 0xa1031, 0
};


inline float
mpllcon_to_freq(struct mpllcon *reg, unsigned fin)
{
	unsigned
		m = reg->mdiv + 8,
		p = reg->pdiv + 2,
		s = reg->sdiv;

	return (float)fin * m / (p << s);
}


int
is_valid_mpllcon(unsigned *reg)
{
	unsigned *p = valid_mpllcon_values;

	// if mpll is set to hp default, return 1
	if (*p == *reg) {
		return 1;
	}

	// otherwise, iterate over mpllcon values
	// and return 2 on the first match
	while (*++p) {
		if (*reg == *p) {
			return 2;
		}
	}

	return 0;
}
