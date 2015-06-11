#include "stdafx.h"
#include <include/diode.h>
#include <common/common.h>

ngled::ngled(string name, double lightCurrent/* = 5e-3*/)
	:ngdevice('X', name, 2, 1)
	,lightCurrent(lightCurrent)
	,status(init)
{
	ngdevice::subckt = "led";
	ngdevice::branches[0] = format_string("v.x%s.v#branch", name.c_str());
}

std::string ngled::card()
{
	return FormatString(100, "%c%s %s %s LED", type, name.c_str(), orders[0].c_str(), orders[1].c_str());
}

bool ngled::action(double time)
{
	bool activated = false;
	if (init == status)
	{
		printf("LED %s is off\n", name.c_str());
		status = off;
		activated = true;
	}

	if (ngdevice::currents[0] >= lightCurrent && off == status)
	{
		status = on;
		printf("LED %s is on. A = %g, time = %g\n", name.c_str(), ngdevice::currents[0], time);
		activated = true;
	}
	else if (ngdevice::currents[0] < lightCurrent && on == status)
	{
		status = off;
		printf("LED %s is off. A = %g, time = %g\n", name.c_str(), ngdevice::currents[0], time);
		activated = true;
	}

	return activated;
}