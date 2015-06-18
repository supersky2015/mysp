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

// spst pack
class ngspst_pack : public ngdevice
{
public:
	ngspst_pack(string name, int pack_count, int st = ngspst::off);

	~ngspst_pack();
		
	// switchover spst_pack.
	// if index less than 0, switchover all spst in the pack.
	// otherwise, only switchover the spst of index in the pack.
	string switchover(int index = all);

	// if index less than 0, connect all spst in the pack.
	// otherwise, only connect the spst of index in the pack.
	string connect(int index = all);

	// if index less than 0, disconnect all spst in the pack.
	// otherwise, only disconnect the spst of index in the pack.
	string disconnect(int index = all);

	string card();

	virtual string& orders(int index);

	virtual ngcontact pin(int p);

	virtual void SetAllowOpen(vector<long> ao);

	enum {all = -1};

	enum {on, off};

private:
	vector<ngspst*> spsts_;

	int status;

	int pack_count_;
};

class ngspdt_pack : public ngdevice
{
public:
	ngspdt_pack(string name, int pack_count, int state = status_throw1);

	~ngspdt_pack();

	string switchover(int index = all);

	string TurnThrow1(int index = all);

	string TurnThrow2(int index = all);

	string card();

	virtual string& orders(int index);

	virtual ngcontact pin(int p);

	virtual void SetAllowOpen(vector<long> ao);

	enum {all = -1};

	enum {status_throw1, status_throw2};

#define p1_throw1	p1
#define p1_pole		p2
#define p1_throw2	p3
#define p2_throw1	p4
#define p2_pole		p5
#define p2_throw2	p6

private:
	vector<ngspdt*> spdts_;

	// which throw the pole connect to
	int status_;

	int pack_count_;
};

#endif