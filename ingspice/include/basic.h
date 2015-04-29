#ifndef BASIC_H
#define BASIC_H

#include <string>
using namespace std;

#include "generic.h"

class ngresistor : public ngdevice
{
public:
	ngresistor(string name, double r)
		:ngdevice(name, 2)
	{
		this->r = r;
	};
	//~ngresistor();

	string netlist();

	double r;
};

class ngcapacitor : public ngdevice
{
public:
	ngcapacitor(string name, double c)
		:ngdevice(name ,2)
	{
		this->c = c;
	};
	//~ngcapacitor();

	string netlist();

	double c;
};

class nginductance : public ngdevice
{
public:
	nginductance(string name, double l)
		:ngdevice(name, 2)
	{
		this->l = l;
	}
	//~nginductance();

	string netlist();

	double l;

};

class ngswitch : public ngdevice
{
public:
	ngswitch(string name, double r)
		:ngdevice(name, 2)
	{
		this->r = r;
	}
	//~ngswitch();

	void connect(){
		r = 1e-10;
	};

	void disconnect(){
		r = 1e20;
	};

	string netlist();

private:
	double r;
};

#endif