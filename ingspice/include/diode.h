#ifndef DIODE_H
#define DIODE_H

#include "generic.h"

class ngled : public ngdevice, public ngaction
{
public:
	ngled(string name)
		:ngdevice(name, 2)
	{
	
	}

	string netlist();

};

#endif