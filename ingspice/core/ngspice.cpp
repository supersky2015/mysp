#include "StdAfx.h"
#include <ngspice/dvec.h> // this should be place before ngspice.h, to prevent redefination
#include "include/ngspice.h"
#include <Windows.h>
#include <algorithm>
#include <assert.h>
#include "common/common.h"

ngspice::ngspice()
	:m_stepCount(0)
	,m_printStep(-1)
	,m_timeToPrint(0.0)
	,m_timeVecIndex(0)
	,m_printTimeStep(-1)
	,m_flagPrompt(false)
	,m_running(false)
	,m_flagCheckLoadCircuit(false)
{
	string path = get_exe_dir();
	SetCurrentDirectoryA(path.c_str());

	static int copy = 1;
	string dll = FormatString(100, "ngspice%d.dll", copy++);
	BOOL success = CopyFileA("ngspice.dll", dll.c_str(), FALSE);
	m_mod = LoadLibrary(dll.c_str());
	ngSpice_Init		= (Func_ngSpice_Init)GetProcAddress(m_mod, "ngSpice_Init");
	ngSpice_Init_Sync	= (Func_ngSpice_Init_Sync)GetProcAddress(m_mod, "ngSpice_Init_Sync");
	ngSpice_Command		= (Func_ngSpice_Command)GetProcAddress(m_mod, "ngSpice_Command");
	ngGet_Vec_Info		= (Func_ngGet_Vec_Info)GetProcAddress(m_mod, "ngGet_Vec_Info");
	ngSpice_Circ		= (Func_ngSpice_Circ)GetProcAddress(m_mod, "ngSpice_Circ");
	ngSpice_CurPlot		= (Func_ngSpice_CurPlot)GetProcAddress(m_mod, "ngSpice_CurPlot");
	ngSpice_AllPlots	= (Func_ngSpice_AllPlots)GetProcAddress(m_mod, "ngSpice_AllPlots");
	ngSpice_AllVecs		= (Func_ngSpice_AllVecs)GetProcAddress(m_mod, "ngSpice_AllVecs");
	ngSpice_running		= (Func_ngSpice_running)GetProcAddress(m_mod, "ngSpice_running");
	ngSpice_SetBkpt		= (Func_ngSpice_SetBkpt)GetProcAddress(m_mod, "ngSpice_SetBkpt");

	int ret = ngSpice_Init(procSendChar, procSendStat, procControlledExit, procSendData, procSendInitData, procBGThreadRunning, this);
}


ngspice::~ngspice(void)
{
	if (m_mod){
		FreeLibrary(m_mod);
		m_mod = NULL;
	}
}

int ngspice::procSendChar( char* str, int id, void* object )
{
	//PRINT("1: %s, %d, 0x%08X\n", str, id, object);
	ngspice* ng = (ngspice*)object;
	
	if (ng->m_flagCheckLoadCircuit || ng->m_running) {
		_strlwr(str);
		if (strstr(str, "error ") || strstr(str, "stderr ")) {
			PRINT("%s\n", str);
			ng->m_errMsgCircuit += str;
			ng->m_errMsgCircuit += "\n";

			// run command error, caller may check running error periodically
			if (ng->m_running){
				ng->m_errRunning = ng->m_errMsgCircuit;
			}
		}
	}

	if (ng->m_flagPrompt)
		PRINT("%s\n", str);

	return 0;
}

int ngspice::procSendStat( char* statusOrValue, int id, void* object )
{
	//PRINT("2: %s, %d, 0x%08X\n", statusOrValue, id, object);
	return 0;
}

int ngspice::procControlledExit( int exitStatus, bool unload, bool quit, int id, void* object )
{
	//PRINT("3: %d, %d, %d, %d, 0x%08X\n", exitStatus, unload, quit, id, object);
	return exitStatus;
}

void ngspice::debugPerSteps( pvecvaluesall actualValues, int number, int steps )
{
	if (steps <= 0)
		return;

	if (actualValues->vecindex - m_stepCount >= steps || 0 == actualValues->vecindex) {
		string values;
		char v[256] = {0};
		for (int i = 0; i < number; i++) {
			sprintf_s(v, 256, "%d: %g; ", i, actualValues->vecsa[i]->creal);
			values += v;
		}
		PRINT(" <%d %d; %s>\n", actualValues->vecindex, actualValues->veccount, values.c_str());
		m_stepCount = actualValues->vecindex;
	}
}

void ngspice::debugPerTime( pvecvaluesall actualValues, int number, double time )
{
	if (time <= 0)
		return;

	if (m_timeVecIndex >= number)
		return;

	double currentTime = actualValues->vecsa[m_timeVecIndex]->creal;
	
	if (currentTime - m_timeToPrint >= time || 0 == actualValues->vecindex)
	{
		string values;
		char v[256] = {0};
		for (int i = 0; i < number; i++) {
			sprintf_s(v, 256, "%d: %g; ", i, actualValues->vecsa[i]->creal);
			values += v;
		}
		PRINT(" <%d %d; %s>\n", actualValues->vecindex, actualValues->veccount, values.c_str());
		m_timeToPrint = currentTime;
	}
}

int ngspice::procSendData( pvecvaluesall actualValues, int number, int id, void* object )
{
	//PRINT("4: 0x%08x, 0x%08x\n", actualValues, actualValues->vecsa);
	//PRINT("4: %d, %d, 0x%08X\n", number, id, object);
	ngspice* ng = (ngspice*)object;

	ng->debugPerSteps(actualValues, number, ng->m_printStep);
	ng->debugPerTime(actualValues, number, ng->m_printTimeStep);

	//get current values of all vector
	for (int i = 0; i < number; i++){
		ng->m_plot.currentValues[i] = actualValues->vecsa[i]->creal;
	}

	//assert(ng->m_plot.currentValues.size() == number);
	//assert(ng->m_plot.names.size() == number);
	//assert(ng->m_plot.pvalues.size() == number);

	//获取向量长度
	ng->m_plot.vecsize = actualValues->vecindex;

	//再一次获取向量值指针，因为计算过程中，计算值向量地址可能会变化
	for (int i = 0; i < number; i++) {
		ng->m_plot.pvalues[i] = ((dvec*)(ng->m_plot.pvecs[i]->pdvec))->v_realdata;

		//运算过程中，计算值向量地址可能会变化
		/*
		if (0 == i) {
			if (ng->m_addressTest.end() == find(ng->m_addressTest.begin(), ng->m_addressTest.end(), (int)ng->m_plot.pvalues[i]))
				ng->m_addressTest.push_back((int)ng->m_plot.pvalues[i]);
		}
		*/
		//assert(ng->m_plot.pvalues[i][actualValues->vecindex] == actualValues->vecsa[i]->creal);
	}

	// 更新器件列表引脚电势，电源电流
	ng->SimAction(ng->CurrentValue("time"));

	return 0;
}

int ngspice::procSendInitData( pvecinfoall initData, int id, void* object )
{
	PRINT("< %s, %s, %s, %s >\n", initData->type, initData->title, initData->name, initData->date);
	ngspice* ng = (ngspice*)object;
	ng->m_stepCount = 0;
	ng->m_timeToPrint = 0.0;

	ng->m_plot.date	= initData->date;
	ng->m_plot.name	= initData->name;
	ng->m_plot.title = initData->title;
	ng->m_plot.type	= initData->type;
	ng->m_plot.veccount = initData->veccount;
	ng->m_plot.pvecs	= initData->vecs;
	ng->m_plot.names.resize(initData->veccount);
	ng->m_plot.pvalues.resize(initData->veccount);
	ng->m_plot.values.resize(initData->veccount);
	ng->m_plot.currentValues.assign(initData->veccount, 0.0);

	for (int i = 0; i < initData->veccount; i++) {
		PRINT("	 %d : %s\n", i, initData->vecs[i]->vecname);
		ng->m_plot.names[i] = initData->vecs[i]->vecname;
		//此时向量值的指针还是空的，需要在procSendData再次获取，也可以间接通过initData->vecs获取
		ng->m_plot.pvalues[i] = ((dvec*)initData->vecs[i]->pdvec)->v_realdata; 

		if (0 == _stricmp(initData->vecs[i]->vecname, "time"))
			ng->m_timeVecIndex = i;
	}

	return 0;
}

int ngspice::procBGThreadRunning( bool running, int id, void* object )
{
	//PRINT("6: %d, %d, 0x%08X\n", running, id, object);
	PRINT(running ? "bg not running\n" : "bg running\n");
	((ngspice*)object)->m_running = !running;
	return 0;
}

int ngspice::procGetVSRCData( double* voltage, double actualTime, char* nodeName, int id, void* object )
{
	//PRINT("7: %f, %f, %s, %d, 0x%08X\n", voltage, actualTime, nodeName, id, object);
	return 0;
}

int ngspice::procGetISRCData( double* current, double actualTime, char* nodeName, int id, void* object )
{
	//PRINT("8: %f, %f, %s, %d, 0x%08X\n", current, actualTime, nodeName, id, object);
	return 0;
}

int ngspice::procGetSyncData( double actualTime, double* deltaTime, double oldDeltaTime, int redoStep, int id, int location, void* object )
{
	//PRINT("9: %f, %f, %f, %d, %d, %d, 0x%08X\n", actualTime, *deltaTime, oldDeltaTime, redoStep, id, location, object);
	return 0;
}

bool ngspice::LoadCircuit( const char* sourceFile /*= NULL*/ )
{
	PRINT("ngspice::LoadCircuit(%s)\n", sourceFile ? sourceFile : "null empty");
	m_errMsgCircuit.clear();
	if (!sourceFile)
		return false;

	string fullname = get_path_fullname(sourceFile);
	string cd = get_current_dir();
	string dstFile = cd + fullname;
	CopyFileA(sourceFile, dstFile.c_str(), FALSE);

	string cmd = FormatString(256, "source %s", fullname.c_str());
	m_flagCheckLoadCircuit = true;
	int ret = ngSpice_Command((char*)cmd.c_str());
	m_flagCheckLoadCircuit = false;

	Do("listing");

	return 0 == m_errMsgCircuit.size();
}

bool ngspice::LoadNetlist( const vector<string>& netlist )
{
	PRINT("ngspice::LoadNetlist(-)\n");
	m_errMsgCircuit.clear();
	size_t count = netlist.size();
	char** circarray = (char**)malloc(sizeof(char*) * count + 1);
	for (size_t i = 0; i < count; i++) {
		circarray[i] = _strdup(netlist[i].c_str());
	}
	circarray[count] = NULL;

	m_flagCheckLoadCircuit = true;
	int ret = ngSpice_Circ(circarray);
	m_flagCheckLoadCircuit = false;

	for (size_t i = 0; i < count; i++) {
		free(circarray[i]);
	}

	// if more the 22 lines, this will stop until key enter pressed.
	Do("listing");

	return 0 == m_errMsgCircuit.size();
}

bool ngspice::Run()
{
	m_errRunning.clear();
	m_errMsgCircuit.clear();
	return !ngSpice_Command("bg_run");
}

bool ngspice::Halt()
{
	return !ngSpice_Command("bg_halt");
}

bool ngspice::Resume()
{
	return !ngSpice_Command("bg_resume");
}

bool ngspice::Restart()
{
	return !ngSpice_Command("bg_halt") && !ngSpice_Command("bg_run");
}

bool ngspice::Do( const char* cmd )
{
	if (!cmd)
		return false;

	PRINT("---- %s ----\n", cmd);
	m_flagPrompt = true;
	bool ret = !ngSpice_Command((char*)cmd);
	m_flagPrompt = false;
	PRINT("\n");
	return ret;
}

bool ngspice::IsRunning()
{
	return m_running;
}

bool ngspice::GetAllPlotsVecs()
{
	vector<plot> vecp;
	char** plots = ngSpice_AllPlots();
	int i = 0, j = 0;
	while (*plots) {
		//skip const plot
		if (!strcmp(*plots, "const"))
			break;

		PRINT(">>> plot%d : %s\n", i++, *plots);

		plot p;
		p.name = *plots;
		char** vecs = ngSpice_AllVecs(*plots);
		while (*vecs) {
			PRINT(" >> vector%d : %s\n", j++, *vecs);
			pvector_info vi = ngGet_Vec_Info(*vecs);
			PRINT("  >type : %d  name : %s  lenght : %d  flag : %d\n", vi->v_type, vi->v_name, vi->v_length, vi->v_flags);
			p.names.push_back(*vecs);
			plot::VecArray vec;
			for (int m = 0; m < vi->v_length; m++) {
				//v.vs.push_back(vi->v_realdata[m]);
				vec.push_back(vi->v_realdata[m]);
			}
			p.values.push_back(vec);
			vecs++;
		}
		plots++;
		j = 0;
		vecp.push_back(p);
	}
	return 0;
}

string ngspice::ErrorMsgLoading()
{
	return m_errMsgCircuit;
}

std::string ngspice::ErrorMsgRuning()
{
	return m_errRunning;
}

inline double ngspice::CurrentValue( string name )
{
	return m_plot.GetVecCurrValue(name);
}
