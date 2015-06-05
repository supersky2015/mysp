#include "stdafx.h"
#include <include/source.h>
#include <common/common.h>

std::string ngdc::card()
{
	return format_string("%c%s %s %s dc %g", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), v);
}


std::string ngac::card()
{
	return format_string("%c%s %s %s sin(%g %g %g %g 0 %g)", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), o, a, f, d, p);
}

std::string ngpluse::card()
{
	return format_string("%c%s %s %s pulse(%g %g %g %g %g %g %g)", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), v1, v2, td, tr, tf, pw, per);
}

string ngdc_current::card()
{
	return format_string("%c%s %s %s dc %g", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), i);
}

string ngac_current::card()
{
	return format_string("%c%s %s %s sin(%g %g %g %g 0 %g)", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), o, a, f, d, p);
}

string ngvcvs::card()
{
	return format_string("%c%s %s %s %s %s %g", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), orders[2].c_str(), orders[3].c_str(), value);
}

string ngcccs::card()
{
	return format_string("%s params: value=%g", ngdevice::subckt_card().c_str(), value);
}

string ngvccs::card()
{
	return format_string("%c%s %s %s %s %s %g", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), orders[2].c_str(), orders[3].c_str(), value);
}

string ngccvs::card()
{
	return format_string("%s params: value=%g", ngdevice::subckt_card().c_str(), value);
}
