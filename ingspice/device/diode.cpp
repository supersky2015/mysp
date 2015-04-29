#include "stdafx.h"
#include <include/diode.h>
#include <common/common.h>

std::string ngled::netlist()
{
	return FormatString(100, "D%s %s %s %s", name.c_str(), orders[0].c_str(), orders[1].c_str());
}
