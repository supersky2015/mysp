#ifndef DIODE_H
#define DIODE_H

#include "generic.h"

class ngled : public ngdevice, public ngaction
{
public:
	ngled(string name, double lightCurrent = 5e-3);

	string netlist();
	
	void action();

	double lightCurrent;	//大于这个值，LED点亮, 默认5mA

private:

	enum{init, off, on};

	int status;
};

#endif