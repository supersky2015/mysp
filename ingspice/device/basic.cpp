#include "stdafx.h"
#include <include/basic.h>
#include <common/common.h>

std::string ngresistor::netlist()
{
	return FormatString(100, "R%s %s %s %f", name.c_str(), orders[0].c_str(), orders[1].c_str(), r);
}

std::string ngcapacitor::netlist()
{
	return FormatString(100, "C%s %s %s %f", name.c_str(), orders[0].c_str(), orders[1].c_str(), c);
}

std::string nginductance::netlist()
{
	return FormatString(100, "L%s %s %s %f", name.c_str(), orders[0].c_str(), orders[1].c_str(), l);
}

std::string ngswitch::netlist()
{
	return FormatString(100, "R%s %s %s %f", name.c_str(), orders[0].c_str(), orders[1].c_str(), r);
}
