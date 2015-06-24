#ifndef DEVICE_SOURCE_H_
#define DEVICE_SOURCE_H_

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
		,v_(v)
	{
	}

	string AlterVoltage(double v);

#define pos p1
#define neg p2

	string card();

	inline double v(){return v_;}

private:
	double v_;
};

class ngac: public ngdevice
{
public:
	ngac(string name, double o, double a, double f, double d = 0.0, double p = 0.0 )
		:ngdevice('V', name, 2)
		,o_(o)
		,a_(a)
		,f_(f)
		,d_(d)
		,p_(p)
	{
	}

	string card();

private:
	double o_;	//offset
	double a_;   //amplitude
	double f_;	//frequency
	double d_;	//delay	
	double p_;	//phase, phase to move sin backward
};

class ngpluse: public ngdevice
{
public:
	// according to manual, td and tr default to TSTEP.
	// test(see. test_555_monostable_running_error) shows they should be bigger than 0. otherwise the simulation crawls slowly, eventually ends up with memory exhausted
	ngpluse(string name, double v1, double v2, double pw, double per, double td, double tr, double tf = 0)
		:ngdevice('V', name, 2)
		,v1_(v1)
		,v2_(v2)
		,td_(td)
		,tr_(tr)
		,tf_(tf)
		,pw_(pw)
		,per_(per)
	{
	}

	string card();

#define pos p1
#define neg p2

private:
	double v1_;	//initial value
	double v2_;	//pulsed value
	double td_;	//delay time
	double tr_;	//rise time
	double tf_;	//fall time
	double pw_;	//pulse width
	double per_;	//period

};

//current source, DC
class ngdc_current : public ngdevice
{
public:
	ngdc_current(string name, double i = 1)
		:ngdevice('I', name, 2)
		,i_(i)
	{
	}

	string card();
	
	string AlterCurrent(double i);

	inline double i(){return i_;}

	//A current source of positive value forces current to flow out of the n+ node, 
	//through the source, and into the n- node
#define CS_POS p1
#define CS_NEG p2

private:
	//current
	double i_;
};

//current source, AC
class ngac_current : public ngdevice
{
public:
	ngac_current(string name, double o, double a, double f, double d = 0.0, double p = 0.0 )
		:ngdevice('I', name, 2)
		,o_(o)
		,a_(a)
		,f_(f)
		,d_(d)
		,p_(p)
	{
	}

	//get card of device.
	string card();

private:
	double o_;	//offset
	double a_;  //amplitude
	double f_;	//frequency
	double d_;	//delay	
	double p_;	//phase, phase to move sin backward
};

//Exxxx: Linear Voltage-Controlled Voltage Sources (VCVS)
class ngvcvs : public ngdevice
{
public:
	ngvcvs(string name, double v = 1.0)
		:ngdevice('E', name, 4)
		,value_(v)
	{
	}

	string card();

#define VCVS_POS	p1	//n+, positive node. Current flow is from the positive node(n+), through the source, to the negative. controlled
#define VCVS_NEG	p2	//n-
#define VCVS_CPOS	p3	//nc+ positive controlling node. 
#define VCVS_CNEG	p4	//nc-

private:
	//value is the voltage gain
	double value_;
};

//Fxxxx: Linear Current-Controlled Current Sources (CCCS)
class ngcccs : public ngdevice
{
public:
	ngcccs(string name, double v = 1.0)
		:ngdevice('X', name, 4)
		,value_(v)
	{
		ngdevice::subckt = "cccs";
	}

	string card();

#define CCCS_CPOS	p1	//nc+ negative controlling node, in subckt. as it is.
#define CCCS_CNEG	p2	//nc- 
#define CCCS_POS	p3	//n+ positive node, controlled output. Current flow is from the positive node(n+), through the source
#define CCCS_NEG	p4	//n-

private:
	//value is the current gain.
	double value_;

	//m is an optional multiplier to the output current.
	double m_;
};

//Gxxxx: Linear Voltage-Controlled Current Sources (VCCS)
class ngvccs : public ngdevice
{
public:
	ngvccs(string name, double v = 1.0)
		:ngdevice('G', name, 4)
		,value_(v)
	{
	}

	string card();

#define VCCS_POS	p1	//n+, positive node. Current flow is from the positive node(n+), through the source, to the negative. controlled
#define VCCS_NEG	p2	//n-
#define VCCS_CPOS	p3	//nc+ positive controlling node. 
#define VCCS_CNEG	p4	//nc-

private:
	//value is the transconductance (in mhos).
	double value_;

	//m is an optional multiplier to the output current
	double m_;
};

//Hxxxx: Linear Current-Controlled Voltage Sources
class ngccvs : public ngdevice
{
public:
	ngccvs(string name, double v = 1.0)
		:ngdevice('X', name, 4)
		,value_(v)
	{
		ngdevice::subckt = "ccvs";
	}

	string card();

#define CCVS_CPOS	p1	//nc+, positive controlling node, in subckt. as it is.
#define CCVS_CNEG	p2	//nc-
#define CCVS_POS	p3	//n+ positive node, controlled output. as it is.
#define CCVS_NEG	p4	//n-

private:
	//value is the transresistance (in ohms).
	double value_;
};

#endif