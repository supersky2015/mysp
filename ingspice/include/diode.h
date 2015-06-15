#ifndef DIODE_H
#define DIODE_H

#include "generic.h"

class ngled : public ngdevice, public ngaction
{
public:
	ngled(string name, double lightCurrent = 5e-3);

	string card();
	
	bool action(double time);

	double lightCurrent;	//大于这个值，LED点亮, 默认5mA

#define pos p1
#define neg p2

	enum{init, off, on};

protected:
	int status;
};

#endif