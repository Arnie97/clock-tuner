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

#ifndef _HP39KBD_H
#define _HP39KBD_H

#include "s3c2410.h"

#define modifier_key_pressed(bit) ((*GPFDAT >> (bit)) & 1)
#define on_pressed    modifier_key_pressed(0)
#define comma_pressed modifier_key_pressed(4)
#define alpha_pressed modifier_key_pressed(5)
#define shift_pressed modifier_key_pressed(6)
#define hp49_alpha_pressed comma_pressed
#define hp49_ls_pressed alpha_pressed
#define hp49_rs_pressed shift_pressed

#define any_normal_key_pressed ((*GPGDAT & 0xFE) != 0xFE)
#define any_key_pressed ( \
	any_normal_key_pressed || on_pressed || \
	comma_pressed || alpha_pressed || shift_pressed \
)

int get_key(void);

#endif
