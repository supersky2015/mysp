#ifndef DEVICE_H
#define DEVICE_H
#include "ingspice.h"

class ngline : public ngdevice
{
public:
	port<2> pins;
};

class ngresistor : public ngdevice
{
public:
	ngresistor(double r){
		this->r = r;
	};
	~ngresistor();

	double r;
	port<2> pins;
};
 
class ngcapacitor : public ngdevice
{
public:
	ngcapacitor(double c);
	~ngcapacitor();

	double c;
	port<2> pins;
};

class nginductance : public ngdevice
{
public:
	nginductance(double l){
		this->l = l;
	}
	~nginductance();

	double l;
	port<2> pins;
};

class ngswitch : public ngdevice
{
public:
	ngswitch(double r){
		this->r = r;
	}
	~ngswitch();

	void connect(){
		r = 1e20;
	};

	void disconnect(){
		r = 1e-10;
	};

	port<2> pins;

private:
	double r;
};

class ngdc : public ngdevice
{
public:
	ngdc(double v){
		this->v = v;
	}
	~ngdc();

	double v;
	port<2> pins;
#define pos pins[0]
#define neg pins[1]
};

class ngac: public ngdevice
{
public:
	ngac(double o, double a, double f, double d = 0 ){
		this->o = o;
		this->a = a;
		this->f = f;
		this->d = d;
	}
	double o;	//offset
	double a;   //amplitude
	double f;	//frequency
	double d;	//delay	
	port<2> pins;
};

class ngled : public ngdevice, public ngaction
{
public:
	
};
#endif
