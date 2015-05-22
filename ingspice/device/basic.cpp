#include "stdafx.h"
#include <include/basic.h>
#include <common/common.h>

const double ngswitch::on = 1e-10;
const double ngswitch::off = 1e20;

std::string ngresistor::card()
{
	return FormatString(100, "%c%s %s %s %g", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), r);
}

std::string ngcapacitor::card()
{
	return FormatString(100, "%c%s %s %s %g", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), c);
}

std::string nginductance::card()
{
	return FormatString(100, "%c%s %s %s %g", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), l);
}

std::string ngswitch::card()
{
	return FormatString(100, "%c%s %s %s %g", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), r);
}

std::string ngswitch::connect()
{
	r = on;
	return format_string("alter r%s=%g", name.c_str(), r);
}

std::string ngswitch::disconnect()
{
	r = off;
	return format_string("alter r%s=%g", name.c_str(), r);
}

std::string ngswitch::switchover()
{
	if (on == r)
		r = off;
	else if (off == r)
		r = on;

	return format_string("alter r%s=%g", name.c_str(), r);
}

ngspst::ngspst( string name, int st/* = ngspst::off*/)
	:ngdevice('X', name, 2)
	,status(st)
{
	ngdevice::subckt = "spst";
}

std::string ngspst::switchover()
{
	status = (on == status) ? off : on;
	return format_string("alter v.x%s.v1=%d", name.c_str(), on == status ? -2 : 0);
}

std::string ngspst::card()
{
	return format_string("%c%s %s %s spst params:vstatus=%d", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), on == status ? -2 : 0 );
}

ngspdt::ngspdt( string name, int st /*= status_throw1*/ )
	:ngdevice('X', name, 3)
	,status(st)
{
	ngdevice::subckt = "spdt";
}

std::string ngspdt::switchover()
{
	status = (status_throw1 == status) ? status_throw2 : status_throw1;
	return format_string("alter v.x%s.v1=%d", name.c_str(), status_throw1 == status ? 0 : -2);
}

std::string ngspdt::card()
{
	return format_string("%c%s %s %s %s spdt params:vstatus=%d", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), orders[2].c_str(), status_throw1 == status ? 0 : -2 );
}
