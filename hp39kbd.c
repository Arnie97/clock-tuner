/* HP 39g+ / 39gs / 40gs series keyboard input module

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

#include "hp39kbd.h"


int
get_key(int (*handler)(unsigned row, unsigned col))
{
	// wait until a key is pressed
	while (!any_key_pressed);

	for (unsigned c = 0; c < 8; c++) {
		// set the current column pin to output, others inputs
		*GPGCON = (1 << 16 << c * 2) | 0xAAA9;
		delay(100);

		// skip this column if no key in it is pressed
		if (!any_key_pressed) {
			continue;
		}
		for (unsigned r = 0; r < 8; r++) {
			// check whether a row is active
			if (!(*GPGDAT & (1 << r + 1))) {
				// restore all column pins to output
				*GPGCON = 0x5555AAA9;
				delay(100);

				// call the event handler and return
				// at the first detected key press
				return handler(r, c);
			}
		}
	}

	// the key was released before it could be captured, hence try again
	return get_key(handler);
}
