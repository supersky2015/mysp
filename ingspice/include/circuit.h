#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <vector>
#include <map>
#include "ngspice.h"

using namespace std;

class schema;
class ngdevice;

class circuit : public ngspice
{
public:
	circuit(schema* sch);
	~circuit(void);

	// 瞬态分析仿真
	//	sch - 待仿真的电路图
	//	step - 仿真步长，默认10us
	//	max - 仿真持续时间，默认1t，1e12s
	bool Tran(const char* max = "1t", const char* step = "10u", const char* ic = " uic");

	// turn on switch
	bool TurnOn(ngdevice* sw);

	// turn off switch
	bool TurnOff(ngdevice* sw);

	// switchover a switch 
	bool SwitchOver(ngdevice* sw);

	// plot in new thread
	// TOFIX: the dialog of plot is stuck now. however it's good enough to visualize a plot.
	bool Plot(string vec);

	// wait until simulation finishing
	bool Wait();

private:
	// a circuit has only a schema
	schema* sch;

	// add control command into netlist.
	bool updateNetlist(vector<string>& netlist, string command);

	// common implementation of TurnOn TurnOff SwitchOver
	enum {switchover, on, off};
	bool turnSwitch(ngdevice* sw, int status = switchover);

	// member function called back by ngspice
	virtual void SimAction(double time);

	// get schema of this circuit
	inline schema* Schema(){return sch;}

	// thread to plot
	static unsigned long __stdcall procPlot(void* param);

	// memory pass to thread procPlot
	shared_ptr<string> vectoplot;
};

#endif
