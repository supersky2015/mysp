#include "stdafx.h"
#include "include/indicators.h"
#include <algorithm>
#include "common/common.h"

ngseven_seg::ngseven_seg( string name, string subckt /*= "seven_seg_com_neg"*/, double light_current/* = 5e-3*/ )
	:ngdevice('X', name, 8, 7)
	,light_current_(light_current)
	,digital_(-1)
	,code_(0x00)
{
	ngdevice::subckt = subckt;
	for (size_t i = 1; i <= 7; i++)
		ngdevice::branches[i - 1] = format_string("v.x%s.v%d#branch", name.c_str(), i);
}

string ngseven_seg::card()
{
	return ngdevice::subckt_card();
}

bool ngseven_seg::action( double time )
{
	static unsigned char code1[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
	static unsigned char code2[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67};

	bool activated = false;

	unsigned char code = 0;
	for (size_t i = 0; i < 7; i++)
	{
		if (currents[i] >= light_current_)
			code |= (1 << i);
	}

	if (code_ != code)
	{
		code_ = code;
		digital_ = -1;
		
		unsigned char* pos = find(code1, code1 + 10, code_);
		if (pos != code1 + 10)
			digital_ = pos - code1;
		else
		{
			pos = find(code2, code2 + 10, code_);
			if (pos != code2 + 10)
				digital_ = pos - code2;
		}

		PRINT("<7seg name=%s code=%02X digital=%d T=%g/>\n", name.c_str(), code_, digital_, time);
		activated = true;
	}

	return activated;
}

std::string ngvoltmeter::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" %g", ohm_);
}

bool ngvoltmeter::action( double time )
{
	bool activated = false;

	// init voltage when time = 0
	if (abs(time) <= TIME_EPSILON)
	{
		PRINT("<voltmeter name=%s voltage=%gV T=%g/>\n", name.c_str(), voltage_, time);
		activated = true;
	}

	// when voltage changes.
	double v = potentials(0) - potentials(1);
	if (abs(v - voltage_) >= VALUE_EPSILON)
	{
		voltage_ = v;
		PRINT("<voltmeter name=%s voltage=%gV T=%g/>\n", name.c_str(), voltage_, time);
		activated = true;
	}

	return activated;
}

ngammeter::ngammeter( string name )
	:ngdevice('V', name, 2, 1)
	,current_(0.0)
{
	ngdevice::branches[0] = format_string("%c%s#branch", type, name.c_str());
}

std::string ngammeter::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + " 0";
}

bool ngammeter::action( double time )
{
	bool activated = false;

	//init ampere when time = 0
	if (abs(time) <= TIME_EPSILON)
	{
		PRINT("<ammeter name=%s ampere=%gA T=%g/>\n", name.c_str(), current_, time);
		activated = true;
	}

	// when ampere changes
	if (abs(ngdevice::currents[0] - current_) >= VALUE_EPSILON)
	{
		current_ = ngdevice::currents[0];
		PRINT("<ammeter name=%s ampere=%gA T=%g/>\n", name.c_str(), current_, time);
		activated = true;
	}

	return activated;
}

string ngac_voltmeter::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" %g", ohm_);
}

bool ngac_voltmeter::action( double time )
{
	bool activated = false;

	// init voltage when time = 0
	// several steps may happen in TIME_EPSILON, ignore as just one step. 
	if (abs(time) <= TIME_EPSILON)
	{
		count_ = 1;
		sum_square_ = 0.0;
		PRINT("<ac_voltmeter name=%s voltage=%gV T=%g/>\n", name.c_str(), voltage_, time);
		activated = true;
	}
	else
		count_++;

	// when voltage changes.
	double dv = (potentials(0) - potentials(1));
	sum_square_ += dv*dv;
	double ev = sqrt(sum_square_/count_);
	if (abs(ev - voltage_) >= VALUE_EPSILON4)
	{
		voltage_ = ev;
		PRINT("<ac_voltmeter name=%s dv=%g voltage=%gV count=%d T=%g/>\n", name.c_str(), dv, voltage_, count_, time);
		activated = true;
	}

	return activated;
}

ngac_ammeter::ngac_ammeter( string name )
	:ngdevice('V', name, 2, 1)
	,current_(0)
	,count_(0)
	,sum_square_(0)
{
	ngdevice::branches[0] = format_string("%c%s#branch", type, name.c_str());
}

string ngac_ammeter::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + " 0";
}

bool ngac_ammeter::action( double time )
{
	bool activated = false;
	// init voltage when time = 0
	// several steps may happen in TIME_EPSILON, ignore as just one step. 
	if (abs(time) <= TIME_EPSILON)
	{
		count_ = 1;
		sum_square_ = 0.0;
		PRINT("<ac_ammeter name=%s current=%gA T=%g/>\n", name.c_str(), current_, time);
		activated = true;
	}
	else
		count_++;

	// when voltage changes.
	double dc = ngdevice::currents[0];
	sum_square_ += dc*dc;
	double ec = sqrt(sum_square_/count_);
	if (abs(ec - current_) >= VALUE_EPSILON4)
	{
		current_ = ec;
		PRINT("<ac_ammeter name=%s dc=%g current=%gA count=%d T=%g/>\n", name.c_str(), dc, current_, count_, time);
		activated = true;
	}

	return activated;
}
