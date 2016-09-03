/* Display manipulating module

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

#ifndef _DISPLAY_H
#define _DISPLAY_H

#define width           131
#define height          64
#define bytes_per_row   20

#include <stdint.h>
extern uint8_t *__display_buf;

#define indicator(n) __display_buf[bytes_per_row * (n) + (width >> 3)]
#define indicator_mask (1 << (width & 7))
#define get_indicator(n) (indicator(n) | indicator_mask)
#define set_indicator(n, value) { \
    if (value) indicator(n) |= indicator_mask; \
    else indicator(n) &= ~indicator_mask; \
};

#define indicator_remote    0
#define indicator_lshift    1
#define indicator_rshift    2
#define indicator_alpha     3
#define indicator_battery   4
#define indicator_wait      5

#endif
