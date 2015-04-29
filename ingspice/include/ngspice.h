#ifndef INGSPICE_H
#define INGSPICE_H
#include <vector>
#include <string>
#include <windows.h>
#include "sharedspice.h"

using namespace std;

/*
* 向量-vec	：在ngspice里面代表一个计算点，包括等势点，电流分支
* 图-plot	：代表一次仿真指令的结果集合，可以通过 .tran .dc .ac 等仿真指令
*/
struct plot
{
	struct vec
	{
		string name;
		double* values;	//一个向量的计算值数组
		vector<double> vs;
	};

	string title;
	string name;
	string date;
	string type;

	int veccount;		//向量的个数, 即pvecs数组的个数
	int vecsize;		//各个向量的计算值的个数，即vec.values数组的个数。注：一个plot里面所有向量都有一样size的value

	pvecinfo* pvecs;

	vector<vec> vecs;
};

class ngspice
{
public:
	ngspice(void);
	~ngspice(void);

public:
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

	//bool m_breakTest;
	//vector<int> m_addressTest;
};

#endif
