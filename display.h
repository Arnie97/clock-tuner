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

#define rows            8
#define cols_storage    8
#define cols_real       9
#define bytes_per_glyph ((rows * cols_storage + 7) / 8)

#define width           131
#define height          64
#define bytes_per_row   20

#define left_margin     3
#define top_margin      1
#define line_spacing    1

extern uint8_t *__display_buf;

#endif
