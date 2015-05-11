#ifndef DIODE_H
#define DIODE_H

#include "generic.h"

class ngled : public ngdevice, public ngaction
{
public:
	ngled(string name, double lightCurrent = 5e-3);

	string card();
	
	void action();

	double lightCurrent;	//大于这个值，LED点亮, 默认5mA

#define pos p1
#define neg p2

private:

	enum{init, off, on};

	int status;
};

#endif