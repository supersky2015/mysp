#ifndef BASIC_H
#define BASIC_H

#include <string>
using namespace std;

#include "generic.h"

class ngresistor : public ngdevice
{
public:
	ngresistor(string name, double r)
		:ngdevice('R', name, 2)
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
		:ngdevice('C', name ,2)
	{
		this->c = c;
	};

	string card();

	double c;
};

class nginductance : public ngdevice
{
public:
	nginductance::nginductance(string name, double l)
		:ngdevice('L', name, 2)
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
		:ngdevice('R', name, 2)
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

class ngspst : public ngdevice
{
public:
	ngspst(string name, int st = ngspst::off);

	string switchover();

	string connect();

	string disconnect();

	string card();

	enum {on, off};

private:
	// spst status, init off
	int status;
};

// 单刀双掷（Single-Pole Double-Throw）
class ngspdt : public ngdevice
{
public:
	ngspdt(string name, int st = status_throw1);

	string switchover();

	string TurnThrow1();

	string TurnThrow2();

	string card();

	enum {status_throw1, status_throw2};

#define throw1	p1
#define pole	p2
#define throw2	p3

private:
	// which throw the pole connect to
	int status;
};

#endif