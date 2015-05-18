#ifndef INGSPICE_H
#define INGSPICE_H
#include <vector>
#include <string>
#include <windows.h>
#include <ngspice/sharedspice.h>
#include "plot.h"

using namespace std;

class ngspice
{
public:
	ngspice();
	~ngspice(void);

	// load circuit from a .cir netlist file.
	bool LoadCircuit(const char* sourceFile = NULL);

	// load circuit from string array netlist.
	bool LoadNetlist(const vector<string>& netlist);

	// run background simulation
	bool Run();

	// halt background simulation
	bool Halt();

	// resume background simulation.
	bool Resume();

	// restart background simulation
	bool Restart();

	// whether running a simulation
	bool IsRunning();

	// get error message of loading a circuit after fail to call LoadCircuit() or LoadNetlist()
	string ErrorMsgLoading();

	// get error message of running a simulation after call Run() or Resume().
	// this error occurs on the callback in the background thread.
	string ErrorMsgRuning();

	// iterator all plots and all vectors, and print their general info. as a debug info.
	bool GetAllPlotsVecs();

	// run a ngspice command
	bool Do(const char* cmd);

	// get a ref to m_plot
	inline plot& GetPlot(){return m_plot;};

	// get current value by vector name
	inline double CurrentValue(string name);

protected:
	// called each step simulation from procSendData
	inline virtual void SimAction(double time){};

	// store simulation result
	plot m_plot;

private:
	// group of callback function declaration
	// general promote
	static int procSendChar(char* str, int id, void* object);
	// here to get stat simulation completion percent.
	static int procSendStat(char* statusOrValue, int id, void* object);
	static int procControlledExit(int exitStatus, bool unload, bool quit, int id, void* object);
	// called each time a step simulated.
	// here to get values of all vectors.
	static int procSendData(pvecvaluesall actualValues, int number, int id, void* object);
	// called after ngSpice_Init is invoked
	static int procSendInitData(pvecinfoall initData, int id, void* object);
	// here to set running status.
	static int procBGThreadRunning(bool running, int id, void* object);
	static int procGetVSRCData(double* voltage, double actualTime, char* nodeName, int id, void* object);
	static int procGetISRCData(double* current, double actualTime, char* nodeName, int id, void* object);
	static int procGetSyncData(double actualTime, double* deltaTime, double oldDeltaTime, int redoStep, int id, int location, void* object);

	// group of interfaces from sharedspice library
	typedef int (*Func_ngSpice_Init)(SendChar* printfcn, SendStat* statfcn, ControlledExit* ngexit, SendData* sdata, SendInitData* sinitdata, BGThreadRunning* bgtrun, void* userData);
	typedef int (*Func_ngSpice_Init_Sync)(GetVSRCData *vsrcdat, GetISRCData *isrcdat, GetSyncData *syncdat, int *ident, void *userData);
	typedef int (*Func_ngSpice_Command)(char* command);
	typedef pvector_info (*Func_ngGet_Vec_Info)(char* vecname);
	typedef int (*Func_ngSpice_Circ)(char** circarray);
	typedef char* (*Func_ngSpice_CurPlot)(void);
	typedef char** (*Func_ngSpice_AllPlots)(void);
	typedef char** (*Func_ngSpice_AllVecs)(char* plotname);
	typedef bool (*Func_ngSpice_running)(void);
	typedef bool (*Func_ngSpice_SetBkpt)(double time);

	// group of proc interface 
	Func_ngSpice_Init		ngSpice_Init;
	Func_ngSpice_Init_Sync	ngSpice_Init_Sync;
	Func_ngSpice_Command	ngSpice_Command;
	Func_ngGet_Vec_Info		ngGet_Vec_Info;
	Func_ngSpice_Circ		ngSpice_Circ;
	Func_ngSpice_CurPlot	ngSpice_CurPlot;
	Func_ngSpice_AllPlots	ngSpice_AllPlots;
	Func_ngSpice_AllVecs	ngSpice_AllVecs;
	Func_ngSpice_running	ngSpice_running;
	Func_ngSpice_SetBkpt	ngSpice_SetBkpt;

	// count of debug messages from callback procSendData
	int m_sendDataDebug;

	// output simulation message from callback procSendChar if true.
	bool m_flagPrompt;

	// check circuit loading message from callback procSendChar if true.
	bool m_flagCheckLoadCircuit;

	// error message of loading circuit
	string m_errMsgCircuit;

	// error message of running simulation
	string m_errRunning;

	// whether running a simulation
	bool m_running;

	// module handle of ngspice.dll 
	HMODULE m_mod;

	// test ngspice
	//bool m_breakTest;
	//vector<int> m_addressTest;
};

#endif
