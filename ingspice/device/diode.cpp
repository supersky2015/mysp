#include "stdafx.h"
#include <include/diode.h>
#include <common/common.h>

ngled::ngled(string name, double lightCurrent/* = 5e-3*/)
	:ngdevice(name, 2, 1)
	,lightCurrent(lightCurrent)
	,status(init)
{
	ngdevice::subckt = "led";
	ngdevice::branches[0] = format_string("v.x%s.v#branch", name.c_str());
}

std::string ngled::card()
{
	return FormatString(100, "X%s %s %s LED", name.c_str(), orders[0].c_str(), orders[1].c_str());
}

void ngled::action(double time)
{
	if (init == status)
	{
		printf("LED %s is off\n", name.c_str());
		status = off;
	}

	if (ngdevice::currents[0] >= lightCurrent && off == status)
	{
		status = on;
		printf("LED %s is on. A = %g, time = %g\n", name.c_str(), ngdevice::currents[0], time);
	}
	else if (ngdevice::currents[0] < lightCurrent && on == status)
	{
		status = off;
		printf("LED %s is off. A = %g, time = %g\n", name.c_str(), ngdevice::currents[0], time);
	}
}