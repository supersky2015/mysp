#include "stdafx.h"
#include <include/transistor.h>
#include <common/common.h>

ngbjt_npn::ngbjt_npn(string name, string model/* = "generic-npn"*/)
	:ngdevice(name, 3)
{
	ngdevice::model = model;
}

std::string ngbjt_npn::card()
{
	return FormatString(100, "Q%s %s %s %s %s", name.c_str(), orders[0].c_str(), orders[1].c_str(), orders[2].c_str(), model.c_str());
}