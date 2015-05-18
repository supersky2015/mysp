#include "stdafx.h"
#include <include/mixed.h>
#include <common/common.h>

ngtimer::ngtimer( string name, string subckt/*="LMC555_TIMER"*/ )
	:ngdevice(name, 8)
{
	ngdevice::subckt = subckt;
}

std::string ngtimer::card()
{
	string crd = format_string("X%s", name.c_str());
	for (size_t i = 0; i < orders.size(); i++)
	{
		//set a unique name if not connected(open)
		if ("-1" == orders[i])
			format_append(crd, " %s_OPEN_%s", name.c_str(), pins[i].c_str());
		else
			format_append(crd, " %s", orders[i].c_str());
	}
	format_append(crd, " %s", subckt.c_str());
	return crd;
}
