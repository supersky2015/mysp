#ifndef I_NGSPICE_H
#define I_NGSPICE_H
#include <string>
#include <vector>
#include <tuple>
using namespace std;

class ngcontact
{
public:
	string name;	//name of a device
	string pin;

	ngcontact(string name, string pin);
	//ngcontact(const ngcontact& contact);
	//ngcontact& operator = (const ngcontact& rhs);
	bool operator == (const ngcontact& rhs);
};

#if 0
class ngline{
public:
	ngline();
	int order;
};

class ngline1 : public ngline
{
public:
	ngcontact c1;
	ngcontact c2;

	//ngline* line1; //由画图类的负责将接线的转换为接到器件引脚
	//ngline* line2;
};

class ngline2 : public ngline
{
public:
	ngcontact c;
	ngline* line;
};

class ngline3 : public ngline
{
public:
	ngline* line1;
	ngline* line2;
};
#else
class ngline
{
public:
	ngcontact c1;
	ngcontact c2;
	int order;

	ngline(string name1, string pin1, string name2, string pin2);
	ngline(const ngcontact& c1, const ngcontact& c2);
};
#endif

class ngaction 
{
public:
	virtual void callback();
};

class ngdevice
{
public:
	ngdevice(string name, int portCount);
	string name;				//name is unique for a device
	string model;
	string subckt;
	vector<string> pins;
	vector<string> orders;
	virtual string netlist();
	ngcontact operator[](int p);
	ngcontact pin(int p);
#define p1 pin(0)
#define p2 pin(1)
#define p3 pin(2)
#define p4 pin(3)
#define p5 pin(4)
#define p6 pin(5)
#define p7 pin(6)
#define p8 pin(7)
#define p9 pin(8)
#define p10 pin(9)
};

#endif