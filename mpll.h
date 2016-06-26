#ifndef _MPLL_H
#define _MPLL_H

struct mpllcon {
	unsigned sdiv:  2;
	unsigned     :  2;
	unsigned pdiv:  6;
	unsigned     :  2;
	unsigned mdiv:  8;
	unsigned     : 12;
};

float mpllcon_to_freq(struct mpllcon *reg);

#endif
