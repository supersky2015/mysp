#include "stdafx.h"
#include <include/source.h>
#include <common/common.h>

std::string ngdc::card()
{
	return FormatString("%c%s %s %s dc %g", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), v);
}


std::string ngac::card()
{
	return FormatString("%c%s %s %s sin(%g %g %g 0 0 %g)", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), o, a*1.41421356, f, d);
}

std::string ngpluse::card()
{
	return format_string("%c%s %s %s pulse(%g %g %g %g %g %g %g)", type, name.c_str(), orders[0].c_str(), orders[1].c_str(), v1, v2, td, tr, tf, pw, per);
}
