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
#define ground p1
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

	string card();
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


	string card();
};

class ngpluse: public ngdevice
{
public:
	// according to manual, td and tr default to TSTEP.
	// test(see. test_555_monostable_running_error) shows they should be bigger than 0. otherwise the simulation crawls slowly, eventually ends up with memory exhausted
	ngpluse(string name, double v1, double v2, double pw, double per, double td, double tr, double tf = 0)
		:ngdevice(name, 2)
		,v1(v1)
		,v2(v2)
		,td(td)
		,tr(tr)
		,tf(tf)
		,pw(pw)
		,per(per)
	{
	}

	string card();

	double v1;	//initial value
	double v2;	//pulsed value
	double td;	//delay time
	double tr;	//rise time
	double tf;	//fall time
	double pw;	//pulse width
	double per;	//period

#define pos p1
#define neg p2
};

#endif