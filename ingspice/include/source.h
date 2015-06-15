#ifndef SOURCE_H
#define SOURCE_H

#include "generic.h"

class ngground : public ngdevice
{
public:
	ngground(string name = "gnd")
		:ngdevice('\0', name, 1)
	{
	}

	string card();

#define ground p1
};

class ngdc : public ngdevice
{
public:
	ngdc(string name, double v = 5)
		:ngdevice('V', name, 2)
	{
		this->v = v;
	}

	double v;

#define pos p1
#define neg p2

	string card();
};

class ngac: public ngdevice
{
public:
	ngac(string name, double o, double a, double f, double d = 0.0, double p = 0.0 )
		:ngdevice('V', name, 2)
	{
		this->o = o;
		this->a = a;
		this->f = f;
		this->d = d;
		this->p = p;
	}

	double o;	//offset
	double a;   //amplitude
	double f;	//frequency
	double d;	//delay	
	double p;	//phase, phase to move sin backward

	string card();
};

class ngpluse: public ngdevice
{
public:
	// according to manual, td and tr default to TSTEP.
	// test(see. test_555_monostable_running_error) shows they should be bigger than 0. otherwise the simulation crawls slowly, eventually ends up with memory exhausted
	ngpluse(string name, double v1, double v2, double pw, double per, double td, double tr, double tf = 0)
		:ngdevice('V', name, 2)
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

//current source, DC
class ngdc_current : public ngdevice
{
public:
	ngdc_current(string name, double i = 1)
		:ngdevice('I', name, 2)
		,i(i)
	{
	}

	string card();

	//current
	double i;

	//A current source of positive value forces current to flow out of the n+ node, 
	//through the source, and into the n- node
#define CS_POS p1
#define CS_NEG p2
};

//current source, AC
class ngac_current : public ngdevice
{
public:
	ngac_current(string name, double o, double a, double f, double d = 0.0, double p = 0.0 )
		:ngdevice('I', name, 2)
	{
		this->o = o;
		this->a = a;
		this->f = f;
		this->d = d;
		this->p = p;
	}

	double o;	//offset
	double a;   //amplitude
	double f;	//frequency
	double d;	//delay	
	double p;	//phase, phase to move sin backward

	//get card of device.
	string card();
};

//Exxxx: Linear Voltage-Controlled Voltage Sources (VCVS)
class ngvcvs : public ngdevice
{
public:
	ngvcvs(string name, double v = 1.0)
		:ngdevice('E', name, 4)
		,value(v)
	{
	}

	//value is the voltage gain
	double value;

	string card();

#define VCVS_POS	p1	//n+, positive node. Current flow is from the positive node(n+), through the source, to the negative. controlled
#define VCVS_NEG	p2	//n-
#define VCVS_CPOS	p3	//nc+ positive controlling node. 
#define VCVS_CNEG	p4	//nc-
};

//Fxxxx: Linear Current-Controlled Current Sources (CCCS)
class ngcccs : public ngdevice
{
public:
	ngcccs(string name, double v = 1.0)
		:ngdevice('X', name, 4)
		,value(v)
	{
		ngdevice::subckt = "cccs";
	}

	//value is the current gain.
	double value;

	//m is an optional multiplier to the output current.
	double m;

	string card();

#define CCCS_CPOS	p1	//nc+ negative controlling node, in subckt. as it is.
#define CCCS_CNEG	p2	//nc- 
#define CCCS_POS	p3	//n+ positive node, controlled output. Current flow is from the positive node(n+), through the source
#define CCCS_NEG	p4	//n-
};

//Gxxxx: Linear Voltage-Controlled Current Sources (VCCS)
class ngvccs : public ngdevice
{
public:
	ngvccs(string name, double v = 1.0)
		:ngdevice('G', name, 4)
		,value(v)
	{
	}

	//value is the transconductance (in mhos).
	double value;

	//m is an optional multiplier to the output current
	double m;

	string card();

#define VCCS_POS	p1	//n+, positive node. Current flow is from the positive node(n+), through the source, to the negative. controlled
#define VCCS_NEG	p2	//n-
#define VCCS_CPOS	p3	//nc+ positive controlling node. 
#define VCCS_CNEG	p4	//nc-
};

//Hxxxx: Linear Current-Controlled Voltage Sources
class ngccvs : public ngdevice
{
public:
	ngccvs(string name, double v = 1.0)
		:ngdevice('X', name, 4)
		,value(v)
	{
		ngdevice::subckt = "ccvs";
	}

	//value is the transresistance (in ohms).
	double value;

	string card();

#define CCVS_CPOS	p1	//nc+, positive controlling node, in subckt. as it is.
#define CCVS_CNEG	p2	//nc-
#define CCVS_POS	p3	//n+ positive node, controlled output. as it is.
#define CCVS_NEG	p4	//n-
};

#endif