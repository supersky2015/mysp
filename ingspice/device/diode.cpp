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

std::string ngled::netlist()
{
	return FormatString(100, "X%s %s %s LED", name.c_str(), orders[0].c_str(), orders[1].c_str());
}

void ngled::action()
{
	if (init == status)
	{
		printf("LED %s is off\n", name.c_str());
		status = off;
	}

	if (ngdevice::currents[0] >= lightCurrent && off == status)
	{
		status = on;
		printf("LED %s is on. A = %f\n", name.c_str(), ngdevice::currents[0]);
	}
	else if (ngdevice::currents[0] < lightCurrent && on == status)
	{
		status = off;
		printf("LED %s is off. A = %f\n", name.c_str(), ngdevice::currents[0]);
	}
}