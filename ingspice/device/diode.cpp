#include "stdafx.h"
#include "include/diode.h"
#include "common/common.h"

ngled::ngled(string name, double light_current/* = 5e-3*/)
	:ngdevice('X', name, 2, 1)
	,light_current_(light_current)
	,status_(off)
{
	ngdevice::subckt = "led";
	ngdevice::branches[0] = format_string("v.x%s.v#branch", name.c_str());
}

std::string ngled::card()
{
	return ngdevice::subckt_card();
}

bool ngled::action(double time)
{
	bool activated = false;
	if (time <= TIME_EPSILON)
	{
		PRINT("LED %s is off\n", name.c_str());
		status_ = off;
		activated = true;
	}

	if (ngdevice::currents[0] >= light_current_ && off == status_)
	{
		status_ = on;
		PRINT("LED %s is on. A = %g, time = %g\n", name.c_str(), ngdevice::currents[0], time);
		activated = true;
	}
	else if (ngdevice::currents[0] < light_current_ && on == status_)
	{
		status_ = off;
		PRINT("LED %s is off. A = %g, time = %g\n", name.c_str(), ngdevice::currents[0], time);
		activated = true;
	}

	return activated;
}

ngdiode::ngdiode( string name, string model/* = "1N4007"*/ )
	:ngdevice('D', name, 2)
{
	ngdevice::model = model;
}

std::string ngdiode::card()
{
	return ngdevice::model_card();
}
