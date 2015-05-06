#ifndef SOURCE_H
#define SOURCE_H

#include "generic.h"

class ngground : public ngdevice
{
public:
	ngground(string name = "gnd")
		:ngdevice(name, 1)
	{
	}
};

class ngdc : public ngdevice
{
public:
	ngdc(string name, double v)
		:ngdevice(name, 2)
	{
		this->v = v;
	}
	//~ngdc();

	double v;

#define pos p1
#define neg p2

	string netlist();
};

class ngac: public ngdevice
{
public:
	ngac(string name, double o, double a, double f, double d = 0 )
		:ngdevice(name, 2)
	{
		this->o = o;
		this->a = a;
		this->f = f;
		this->d = d;
	}
	double o;	//offset
	double a;   //amplitude
	double f;	//frequency
	double d;	//delay	


	string netlist();
};


#endif