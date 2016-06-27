/* A simple user interface for this project

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
#include <hpgraphics.h>
#include "s3c2410.h"
#include "mpllcon.h"
#include "clkslow.h"


void
show_system_info(void)
{
	clear_screen();
	printf(
		"SYSTEM INFORMATION\n\n"
		"Build 20160627 by Arnie97\n\n"
	);
	for (int i = 0; i < 6; i++) {
		delay(500000);
		hpg_set_indicator((i + 1) % 6, 0xFF);
		hpg_set_indicator(i, 0x00);
	}

	int status;
	float fclk = clkslow_to_freq(CLKSLOW, FIN);
	if (fclk) {
		// slow mode
		status = 4;
	} else {
		// normal mode
		fclk = mpllcon_to_freq(MPLLCON, FIN);
		status = is_valid_mpllcon(MPLLCON);
	}
	const char *msg[] = {
		"UNKNOWN", "DEFAULT", "CHANGED", "PROFILE",
		"SLOW MODE", "ENABLED"
	};
	printf(
		"MPLLCON %08x at %08x\n"
		"CLKSLOW %08x at %08x\n"
		"  (%.2f MHz, %s %s)\n"
		"\n",
		*MPLLCON, (unsigned)MPLLCON,
		*CLKSLOW, (unsigned)CLKSLOW,
		fclk, msg[status], msg[status == 4? 5: 3]
	);

	delay(750000);
	hpg_set_indicator(0, 0x00);
	printf("PRESS [APLET] KEY TO EXIT");
	while (!keyb_isAnyKeyPressed());
}
