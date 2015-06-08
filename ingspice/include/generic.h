#ifndef I_NGSPICE_H
#define I_NGSPICE_H
#include <string>
#include <vector>
#include <tuple>
using namespace std;

#define TIME_EPSILON	1e-5
#define VALUE_EPSILON	1e-5

// a contact class with a name of device and a name of pin
class ngcontact
{
public:
	string name;	//name of a device
	string pin;

	ngcontact(string name, string pin);
	bool operator == (const ngcontact& rhs);
};

// a line class with two contacts
class ngline
{
public:
	ngcontact c1;
	ngcontact c2;

	// sequence of potential node
	int order;

	ngline(string name1, string pin1, string name2, string pin2);
	ngline(const ngcontact& c1, const ngcontact& c2);
};

// base class if device has animation action
class ngaction 
{
public:
	virtual void action(double time) = 0;
};

// base class of all kinds of devices
class ngdevice
{
public:
	// portCount means how many port this device has.
	// branchCount means how many current branch this device has.
	ngdevice(char type, string name, int portCount, int branchCount = 0);
	
	// device type, first letter of card
	char type;

	// name is unique for a device
	string name;

	// 如果器件是个模型， 在这指定模型名，（schema类根据model从配置文件中读进模型card)
	string model;

	// 如果器件是个子电路，在这指定子电路名（schema类根据subckt从配置文件读进子电路card)
	string subckt;

	//器件引脚名, 可以自定义
	vector<string> pins;

	//器件引脚在netlist中分配的电势点序列， 如 "R1 1 2 470" 中的 1 和 2
	vector<string> orders;		

	//器件引脚当前电势值
	vector<double> potentials;	

	//器件包含的电流分支
	vector<string> branches;		

	//器件各分支当前电流值
	vector<double> currents;	
	
	// 获取第p个引脚的触点	
	ngcontact operator[](int p);
	ngcontact pin(int p);

	// 生成器件对应的行，ngspice的传统称之为card
	virtual string card();

	// get card for subckt device
	string subckt_card();

	// get card for model device
	string model_card();

	// get vec name of a contact
	string vec(const ngcontact& contact);

	//get nth vec name
	string vec(size_t n);

	// get branch name if device is a source
	string branch();

	// get branch name by sub component's name and type if device is a subckt.
	string branch(string subType, string subName);

	// predefined pin contact
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