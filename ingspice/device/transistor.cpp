#include "stdafx.h"
#include <include/transistor.h>
#include <common/common.h>

std::string ngbjt_npn::card()
{
	return FormatString(100, "Q%s %s %s %s %s", name.c_str(), orders[0].c_str(), orders[1].c_str(), orders[2].c_str(), model.c_str());
}
