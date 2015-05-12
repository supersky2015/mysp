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
	output_debug_message(" <c: %s, (%d, %d), (%1.3e, %1.3e)>", name.c_str(), orders[0].c_str(), orders[1].c_str(), potentials[0], potentials[1]);
	return FormatString(100, "C%s %s %s %1.3e", name.c_str(), orders[0].c_str(), orders[1].c_str(), c);
	//return FormatString(100, "C%s %s %s %1.3e ic=%1.3e", name.c_str(), orders[0].c_str(), orders[1].c_str(), c, potentials[1] - potentials[0]);
}

nginductance::nginductance(string name, double l)
	:ngdevice(name, 2/*, 1*/)
{
	this->l = l;
	//ngdevice::subckt = "inductance";
	//ngdevice::branches[0] = format_string("v.x%s.v#branch", name.c_str());
}

std::string nginductance::card()
{
	return FormatString(100, "L%s %s %s %1.3e", name.c_str(), orders[0].c_str(), orders[1].c_str(), l);
	//return FormatString(256, "x%s %s %s sub_l_%s\n.subckt sub_l_%s K A\nV K 1 0\nL1 1 A %1.3e ic=%1.3e\n.ends", 
	//	name.c_str(), orders[0].c_str(), orders[1].c_str(), name.c_str(), name.c_str(), branches[0]);
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

ngspst::ngspst( string name, int st/* = ngspst::off*/)
	:ngdevice(name, 2)
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
	return format_string("X%s %s %s spst params:vstatus=%d", name.c_str(), orders[0].c_str(), orders[1].c_str(), on == status ? -2 : 0 );
}

ngspdt::ngspdt( string name, int st /*= status_throw1*/ )
	:ngdevice(name, 3)
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
	return format_string("X%s %s %s %s spdt params:vstatus=%d", name.c_str(), orders[0].c_str(), orders[1].c_str(), orders[2].c_str(), status_throw1 == status ? 0 : -2 );
}
