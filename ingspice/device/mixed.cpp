#include "stdafx.h"
#include <include/mixed.h>
#include <common/common.h>

ngtimer::ngtimer( string name, string subckt/*="LMC555_TIMER"*/ )
	:ngdevice('X', name, 8)
{
	ngdevice::subckt = subckt;
}

std::string ngtimer::card()
{
	return ngdevice::subckt_card();
}
