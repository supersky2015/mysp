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

		printf("<7seg name=%s code=%02X digital=%d T=%g/>\n", name.c_str(), code, digital, time);
	}
}

std::string ngvoltmeter::card()
{
	return format_string("%c%s %s %s %g", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), ohm);
}

void ngvoltmeter::action( double time )
{
	// init voltage when time = 0
	if (abs(time) <= TIME_EPSILON)
	{
		printf("<voltmeter name=%s voltage=%gV T=%g/>\n", name.c_str(), voltage, time);
	}

	// when voltage changes.
	double v = potentials[0] - potentials[1];
	if (abs(v - voltage) >= VALUE_EPSILON)
	{
		voltage = v;
		printf("<voltmeter name=%s voltage=%gV T=%g/>\n", name.c_str(), voltage, time);
	}
}

ngammeter::ngammeter( string name )
	:ngdevice('V', name, 2, 1)
	,current(0.0)
{
	ngdevice::branches[0] = format_string("%c%s#branch", type, name.c_str());
}

std::string ngammeter::card()
{
	return format_string("%c%s %s %s 0", type, name.c_str(), orders[0].c_str(), orders[1].c_str());
}

void ngammeter::action( double time )
{
	//init ampere when time = 0
	if (abs(time) <= TIME_EPSILON)
	{
		printf("<ammeter name=%s ampere=%gA T=%g/>\n", name.c_str(), current, time);
	}

	// when ampere changes
	if (abs(ngdevice::currents[0] - current) >= VALUE_EPSILON)
	{
		current = ngdevice::currents[0];
		printf("<ammeter name=%s ampere=%gA T=%g/>\n", name.c_str(), current, time);
	}
}

string ngac_voltmeter::card()
{
	return format_string("%c%s %s %s %g", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), ohm);
}

void ngac_voltmeter::action( double time )
{
	// init voltage when time = 0
	// several steps may happen in TIME_EPSILON, ignore as just one step. 
	if (abs(time) <= TIME_EPSILON)
	{
		count = 1;
		sum_square = 0.0;
		printf("<ac_voltmeter name=%s voltage=%gV T=%g/>\n", name.c_str(), voltage, time);
	}
	else
		count++;

	// when voltage changes.
	double dv = (potentials[0] - potentials[1]);
	sum_square += dv*dv;
	double ev = sqrt(sum_square/count);
	if (abs(ev - voltage) >= VALUE_EPSILON)
	{
		voltage = ev;
		printf("<ac_voltmeter name=%s dv=%g voltage=%gV count=%d T=%g/>\n", name.c_str(), dv, voltage, count, time);
	}
}

ngac_ammeter::ngac_ammeter( string name )
	:ngdevice('V', name, 2, 1)
{
	ngdevice::branches[0] = format_string("%c%s#branch", type, name.c_str());
}

string ngac_ammeter::card()
{
	return format_string("%c%s %s %s 0", type, name.c_str(), orders[0].c_str(), orders[1].c_str());
}

void ngac_ammeter::action( double time )
{
	// init voltage when time = 0
	// several steps may happen in TIME_EPSILON, ignore as just one step. 
	if (abs(time) <= TIME_EPSILON)
	{
		count = 1;
		sum_square = 0.0;
		printf("<ac_ammeter name=%s current=%gA T=%g/>\n", name.c_str(), current, time);
	}
	else
		count++;

	// when voltage changes.
	double dc = ngdevice::currents[0];
	sum_square += dc*dc;
	double ec = sqrt(sum_square/count);
	if (abs(ec - current) >= VALUE_EPSILON)
	{
		current = ec;
		printf("<ac_ammeter name=%s dc=%g current=%gA count=%d T=%g/>\n", name.c_str(), dc, current, count, time);
	}
}
