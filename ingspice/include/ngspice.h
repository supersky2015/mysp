#ifndef INGSPICE_H
#define INGSPICE_H
#include <vector>
#include <string>
#include <windows.h>
#include "sharedspice.h"
#include "plot.h"
#include "circuit.h"

using namespace std;

class ngspice
{
public:
	ngspice(circuit* cir = NULL, FuncAction action = NULL);
	~ngspice(void);

	bool LoadCircuit(const char* sourceFile = NULL);
	bool LoadNetlist(const vector<string>& netlist);

	bool Run();
	bool Halt();
	bool Resume();
	bool IsRunning();

	string GetErrorMessage();

	bool GetAllPlotsVecs();

	bool Do(const char* cmd);

	inline plot& GetPlot(){return m_plot;};


private:
	bool m_running;

private:
	HMODULE m_mod;

private:
	static int procSendChar(char* str, int id, void* object);
	static int procSendStat(char* statusOrValue, int id, void* object);
	static int procControlledExit(int exitStatus, bool unload, bool quit, int id, void* object);
	static int procSendData(pvecvaluesall actualValues, int number, int id, void* object);
	static int procSendInitData(pvecinfoall initData, int id, void* object);
	static int procBGThreadRunning(bool running, int id, void* object);
	static int procGetVSRCData(double* voltage, double actualTime, char* nodeName, int id, void* object);
	static int procGetISRCData(double* current, double actualTime, char* nodeName, int id, void* object);
	static int procGetSyncData(double actualTime, double* deltaTime, double oldDeltaTime, int redoStep, int id, int location, void* object);

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

	plot m_plot;

	int m_sendDataDebug;
	bool m_flagPrompt;
	bool m_flagCheckLoadCircuit;
	bool m_successLoadCircuit;
	string m_errMsgCircuit;

	// callback of circuit
	FuncAction m_action;

	// instance of circuit
	circuit* m_cir;

	//bool m_breakTest;
	//vector<int> m_addressTest;
};

#endif
