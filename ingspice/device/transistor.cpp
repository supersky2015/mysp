#include "stdafx.h"
#include "include/transistor.h"
#include "common/common.h"

ngbjt_npn::ngbjt_npn(string name, string model/* = "generic-npn"*/)
	:ngdevice('Q', name, 3)
{
	ngdevice::model = model;
}

std::string ngbjt_npn::card()
{
	return ngdevice::model_card();
}