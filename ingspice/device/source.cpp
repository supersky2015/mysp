#include "stdafx.h"
#include <include/source.h>
#include <common/common.h>

std::string ngdc::netlist()
{
	return FormatString(100, "V%s %s %s dc %f", name.c_str(), orders[0].c_str(), orders[1].c_str(), v);
}


std::string ngac::netlist()
{
	return FormatString(512, "V%s %s %s sin(%f %f %f 0 0 %f)", name.c_str(), orders[0].c_str(), orders[1].c_str(), o, a*1.41421356, f, d);
}
