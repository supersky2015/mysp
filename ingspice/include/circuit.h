#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <vector>
#include <map>
using namespace std;

class schema;
class ngspice;
class ngdevice;

class circuit
{
public:
	circuit(schema* sch);
	~circuit(void);

	// if circuit is running simulation
	bool IsRunning() const;

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

	// run simulation
	bool Run();

	// halt simulation
	bool Stop();

	// resume simulation
	bool Resume();

	// halt simulation and run again
	bool Restart();

	// do a command
	bool Do(string cmd);

//private:
	// a circuit has only a schema
	schema* sch;

	// a circuit uses a ngspice to sim a schema.
	ngspice* ng;

	// add control command into netlist.
	bool updateNetlist(vector<string>& netlist, string command);

	// common implementation of TurnOn TurnOff SwitchOver
	enum {switchover, on, off};
	bool turnSwitch(ngdevice* sw, int status = switchover);

	// member function called back by ngspice
	void schemaAction(ngspice* ng);

	// get schema of this circuit
	inline schema* Schema(){return sch;}
};

// circuit member function pointer
typedef void (circuit::*FuncAction)(ngspice* ng);

#endif
