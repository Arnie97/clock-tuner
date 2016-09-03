/* S3C2410 hardware definitions

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

// remapped addresses of registers
#define GPFDAT ((unsigned *)0x07A00054)
#define GPGCON ((unsigned *)0x07A00060)
#define GPGDAT ((unsigned *)0x07A00064)

#endif
