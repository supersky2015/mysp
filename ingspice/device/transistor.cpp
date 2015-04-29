#include "stdafx.h"
#include <include/transistor.h>
#include <common/common.h>

std::string ngbjt_npn::netlist()
{
	return FormatString(100, "Q%s %s %s %s %s", name.c_str(), coll.c_str(), base.c_str(), emit.c_str(), model.c_str());
}
