#ifndef TRANSISTOR_H
#define TRANSISTOR_H

#include "generic.h"

class ngbjt_npn : public ngdevice
{
public:
	ngbjt_npn(string name, string model = "generic-npn")
		:ngdevice(name, 3)
	{
		ngdevice::model = model;
	}

	string netlist();

#define coll pins[0]
#define base pins[1]
#define emit pins[2]
};

#endif