#include "stdafx.h"
#include <algorithm>
#include <include/indicators.h>
#include <common/common.h>

ngseven_seg::ngseven_seg( string name, string subckt /*= "seven_seg_com_neg"*/, double lightCurrent/* = 5e-3*/ )
	:ngdevice('X', name, 8, 7)
	,lightCurrent(lightCurrent)
	,digital(-1)
	,code(0x00)
{
	ngdevice::subckt = subckt;
	for (size_t i = 1; i <= 7; i++)
		ngdevice::branches[i - 1] = format_string("v.x%s.v%d#branch", name.c_str(), i);
}

string ngseven_seg::card()
{
	return ngdevice::subckt_card();
}

void ngseven_seg::action( double time )
{
	static unsigned char code1[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
	static unsigned char code2[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67};

	unsigned char cd = 0;
	for (size_t i = 0; i < 7; i++)
	{
		if (currents[i] >= lightCurrent)
			cd |= (1 << i);
	}

	if (code != cd)
	{
		code = cd;
		digital = -1;
		
		unsigned char* pos = find(code1, code1 + 10, code);
		if (pos != code1 + 10)
			digital = pos - code1;
		else
		{
			pos = find(code2, code2 + 10, code);
			if (pos != code2 + 10)
				digital = pos - code2;
		}

		printf("<7seg name=%s code=%02X digital=%d/>\n", name.c_str(), code, digital);
	}
}