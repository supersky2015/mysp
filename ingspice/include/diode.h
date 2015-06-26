#ifndef DEVICE_DIODE_H_
#define DEVICE_DIODE_H_

#include "generic.h"

class ngled : public ngdevice, public ngaction
{
public:
	ngled(string name, double light_current = 5e-3);

	string card();
	
	bool action(double time);

	inline int status(){return status_;}

#define pos p1
#define neg p2

	enum{off, on};

private:
	int status_;

	double light_current_;	//大于这个值，LED点亮, 默认5mA
};

class ngdiode : public ngdevice
{
public:
	ngdiode(string name, string model = "1N4007");

	string card();
};

#endif