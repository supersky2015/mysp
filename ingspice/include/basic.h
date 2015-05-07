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

	string card();

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

	string card();

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

	string card();

	double l;

};

class ngswitch : public ngdevice
{
public:
	ngswitch(string name, double r = off)
		:ngdevice(name, 2)
	{
		this->r = r;
	}

	string connect();

	string disconnect();

	string switchover();

	string card();

	const static double on;
	const static double off;

private:
	double r;
};

#endif