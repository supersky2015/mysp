#include "stdafx.h"
#include <include/basic.h>
#include <common/common.h>

const double ngswitch::on = 1e-10;
const double ngswitch::off = 1e20;

std::string ngresistor::card()
{
	return FormatString(100, "R%s %s %s %1.3e", name.c_str(), orders[0].c_str(), orders[1].c_str(), r);
}

std::string ngcapacitor::card()
{
	return FormatString(100, "C%s %s %s %1.3e", name.c_str(), orders[0].c_str(), orders[1].c_str(), c);
}

std::string nginductance::card()
{
	return FormatString(100, "L%s %s %s %1.3e", name.c_str(), orders[0].c_str(), orders[1].c_str(), l);
}

std::string ngswitch::card()
{
	return FormatString(100, "R%s %s %s %1.3e", name.c_str(), orders[0].c_str(), orders[1].c_str(), r);
}

std::string ngswitch::connect()
{
	r = on;
	return format_string("alter r%s=%1.3e", name.c_str(), r);
}

std::string ngswitch::disconnect()
{
	r = off;
	return format_string("alter r%s=%1.3e", name.c_str(), r);
}

std::string ngswitch::switchover()
{
	if (on == r)
		r = off;
	else if (off == r)
		r = on;

	return format_string("alter r%s=%1.3e", name.c_str(), r);
}
