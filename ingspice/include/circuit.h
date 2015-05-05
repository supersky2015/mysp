#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <vector>
#include <map>
using namespace std;

class schema;
class ngspice;

class circuit
{
public:
	circuit(schema* sch);
	~circuit(void);

	bool IsRunning() const;

	/*
	 * 功能：瞬态分析仿真
	 * 参数:
	 *		sch - 待仿真的电路图
	 *		step - 仿真步长，默认10us
	 *		max - 仿真持续时间，默认1t，1e12s
	 * 返回:
	 *		
	 */
	bool Tran(const char* max = "1t", const char* step = "10u");

	bool Run();
	bool Stop();
	bool Resume();
	bool Restart();

	
private:
	// a circuit has only a schema
	schema* sch;

	// a circuit uses a ngspice to sim a schema.
	ngspice* ng;

	// add control command into netlist.
	bool updateNetlist(vector<string>& netlist, string command);

	// member function called back by ngspice
	void schemaAction(ngspice* ng);

	inline schema* Schema(){return sch;}
};

typedef void (circuit::*FuncAction)(ngspice* ng);

#endif
