#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <windows.h>  // for Sleep

#include <ngspice/sharedspice.h>

namespace test2
{

typedef int (*FngSpice_Init)(SendChar* printfcn, SendStat* statfcn, ControlledExit* ngexit, SendData* sdata, SendInitData* sinitdata, BGThreadRunning* bgtrun, void* userData);
typedef int (*FngSpice_Init_Sync)(GetVSRCData *vsrcdat, GetISRCData *isrcdat, GetSyncData *syncdat, int *ident, void *userData);
typedef int (*FngSpice_Command)(char* command);
typedef pvector_info (*FngGet_Vec_Info)(char* vecname);
typedef int (*FngSpice_Circ)(char** circarray);
typedef char* (*FngSpice_CurPlot)(void);
typedef char** (*FngSpice_AllPlots)(void);
typedef char** (*FngSpice_AllVecs)(char* plotname);
typedef bool (*FngSpice_running)(void);
typedef bool (*FngSpice_SetBkpt)(double time);




bool no_bg = true;
int vecgetnumber = 0;
int vec_current_number = 0;
double v2dat;
static bool has_break = false;
int testnumber = 0;
void alterp(int sig);
static bool errorflag = false;

int
ng_getchar(char* outputreturn, int ident, void* userdata);

int
ng_getstat(char* outputreturn, int ident, void* userdata);

int
ng_exit(int, bool, bool, int ident, void*);

int
ng_thread_runs(bool noruns, int ident, void* userdata);

int
ng_initdata(pvecinfoall intdata, int ident, void* userdata);

int
ng_data(pvecvaluesall vdata, int numvecs, int ident, void* userdata);

int
cieq(register char *p, register char *s);

FngSpice_Init ngSpice_Init;
FngSpice_Init_Sync ngSpice_Init_Sync;
FngSpice_Command ngSpice_Command;
FngGet_Vec_Info ngGet_Vec_Info;
FngSpice_Circ ngSpice_Circ;
FngSpice_CurPlot ngSpice_CurPlot;
FngSpice_AllPlots ngSpice_AllPlots;
FngSpice_AllVecs ngSpice_AllVecs;
FngSpice_running ngSpice_running;
FngSpice_SetBkpt ngSpice_SetBkpt;

int test_sim_alter()
{

	HMODULE mod = LoadLibrary("ngspice.dll");
	ngSpice_Init = (FngSpice_Init)GetProcAddress(mod, "ngSpice_Init");
	ngSpice_Init_Sync = (FngSpice_Init_Sync)GetProcAddress(mod, "ngSpice_Init_Sync");
	ngSpice_Command = (FngSpice_Command)GetProcAddress(mod, "ngSpice_Command");
	ngGet_Vec_Info = (FngGet_Vec_Info)GetProcAddress(mod, "ngGet_Vec_Info");
	ngSpice_Circ = (FngSpice_Circ)GetProcAddress(mod, "ngSpice_Circ");
	ngSpice_CurPlot = (FngSpice_CurPlot)GetProcAddress(mod, "ngSpice_CurPlot");
	ngSpice_AllPlots = (FngSpice_AllPlots)GetProcAddress(mod, "ngSpice_AllPlots");
	ngSpice_AllVecs = (FngSpice_AllVecs)GetProcAddress(mod, "ngSpice_AllVecs");
	ngSpice_running = (FngSpice_running)GetProcAddress(mod, "ngSpice_running");
	ngSpice_SetBkpt = (FngSpice_SetBkpt)GetProcAddress(mod, "ngSpice_SetBkpt");

    int ret, i;
    char ** circarray;
  
    ret = ngSpice_Init(ng_getchar, ng_getstat, ng_exit,  ng_data, ng_initdata, ng_thread_runs, NULL);

    printf("Init thread returned: %d\n", ret);

    testnumber = 3;
    printf("\n**  Test no %d with flag for stopping background thread  **\n\n", testnumber);
	int pos = 0;

#define CIRCUIT 0 

#if CIRCUIT//test subckt as spst switch
	int arraysize = 13;
    circarray = (char**)malloc(sizeof(char*) * arraysize);
    circarray[pos++] = strdup("test halt to switch and then resume");
    circarray[pos++] = strdup("v1 1 0 dc 5");
	// a single pole single throw switch subckt
    circarray[pos++] = strdup("x1 1 2 spst");
	circarray[pos++] = strdup("r1 2 0 5");
	// last 10s
    circarray[pos++] = strdup(".tran 1m 1000");
	//subckt spst start
	circarray[pos++] = strdup(".subckt spst 1 2 params: ron=1e-8 roff=1e30");
	circarray[pos++] = strdup("r1 0 6 20");
	circarray[pos++] = strdup("v1 6 0 dc 0");
	circarray[pos++] = strdup("w1 1 2 v1 no_contact");
	circarray[pos++] = strdup(".model no_contact csw (it=0.05 ih=0.025 ron={ron} roff={roff})");
	circarray[pos++] = strdup(".ends");
#else//test resistor as spst switch.
	int arraysize = 7;
	circarray = (char**)malloc(sizeof(char*) * arraysize);
	circarray[pos++] = strdup("test resistor as switch");
	circarray[pos++] = strdup("v1 1 0 dc 5");
	circarray[pos++] = strdup("r1 1 2 5");
	circarray[pos++] = strdup("r2 2 0 1e20");
	circarray[pos++] = strdup(".tran 1m 1000");
#endif
	//subckt spst end
	circarray[pos++] = strdup(".end");
	circarray[pos++] = NULL;

    has_break = false;


    ret = ngSpice_Circ(circarray);
    ret = ngSpice_Command("bg_run");
	//ret = ngSpice_Command("listing");


	for(i = 0; i < arraysize; i++)
		free(circarray[i]);
	free(circarray);

#if 0
    /* wait until simulation stops */
    for (;;) {
        Sleep (100);

		// break when time is 5s
        if (has_break) {
            ret = ngSpice_Command("bg_halt");
            ret = ngSpice_Command("listing");
			// set the subckt current 2/20 = 0.1A, to make spst connected
#if CIRCUIT
            ret = ngSpice_Command("alter v.x1.v1=-2");
#else
			ret = ngSpice_Command("alter r2=1e-8");
#endif
			ret = ngSpice_Command("listing");
            ret = ngSpice_Command("bg_resume");
        }

        if (no_bg)
            break;
    }

    /* wait until simulation finishes */
    for (;;) {
        Sleep (100);
        if (no_bg)
            break;
    }
#else
	for (;;) {
		char ch = getchar();
		if (ch == 'a')
		{
			static bool status = false;
			ret = ngSpice_Command("bg_halt");
			ret = ngSpice_Command("listing");
			// set the subckt current 2/20 = 0.1A, to make spst connected
			status = !status;
#if CIRCUIT
			if (status)
				ret = ngSpice_Command("alter v.x1.v1=-2");
			else
				ret = ngSpice_Command("alter v.x1.v1=0");
#else
			if (status)
				ret = ngSpice_Command("alter r2=1e-8");
			else
				ret = ngSpice_Command("alter r2=1e20");
#endif
			ret = ngSpice_Command("listing");
			ret = ngSpice_Command("bg_resume");
		}

		Sleep(100);
		if (no_bg)
			break;
	}

#endif
    //ret = ngSpice_Command("write test3.raw V(2)");

    return 0;
}

int
ng_getchar(char* outputreturn, int ident, void* userdata)
{
    //printf("1: %s\n", outputreturn);
    return 0;
}


int
ng_getstat(char* outputreturn, int ident, void* userdata)
{
    printf("2: %s\n", outputreturn);
    return 0;
}

int
ng_thread_runs(bool noruns, int ident, void* userdata)
{
    no_bg = noruns;
    if (noruns)
        printf("bg not running\n");
    else
        printf("bg running\n");

    return 0;
}

/* Callback function called from bg thread in ngspice once per accepted data point */
int
ng_data(pvecvaluesall vdata, int numvecs, int ident, void* userdata)
{
    int *ret;

	static double current = -1;
	if (current != vdata->vecsa[vec_current_number]->creal)
		printf(" << current = %1.3e >>\n", vdata->vecsa[vec_current_number]->creal);

	current = vdata->vecsa[vec_current_number]->creal;

    v2dat = vdata->vecsa[vecgetnumber]->creal;
	//when the time is greter than 5s, trigger the alter action
    if (!has_break && (v2dat > 5)) {
        has_break = true;
        printf("Pause requested, 5s reached\n");
    /* leave bg thread for a while to allow halting it from main */
        //Sleep (100);
//		ret = ((int * (*)(char*)) ngSpice_Command_handle)("bg_halt");
    }
    return 0;
}


/* Callback function called from bg thread in ngspice once upon intialization
   of the simulation vectors)*/
int
ng_initdata(pvecinfoall intdata, int ident, void* userdata)
{
    int i;
    int vn = intdata->veccount;
    for (i = 0; i < vn; i++) {
        printf("Vector: %s\n", intdata->vecs[i]->vecname);
        /* find the location of V(2) */
        if (cieq(intdata->vecs[i]->vecname, "time"))
            vecgetnumber = i;
		if (cieq(intdata->vecs[i]->vecname, "v1#branch"))
			vec_current_number = i;
    }
    return 0;
}


/* Callback function called from bg thread in ngspice if fcn controlled_exit()
   is hit. Do not exit, but unload ngspice. */
int
ng_exit(int exitstatus, bool immediate, bool quitexit, int ident, void* userdata)
{

    if(quitexit) {
        printf("DNote: Returned form quit with exit status %d\n", exitstatus);
        exit(exitstatus);
    }
    if(immediate) {
        printf("DNote: Unloading ngspice inmmediately is not possible\n");
        printf("DNote: Can we recover?\n");
    }

    else {
        printf("DNote: Unloading ngspice is not possible\n");
        printf("DNote: Can we recover? Send 'quit' command to ngspice.\n");
        errorflag = true;
        ngSpice_Command("quit 5");
//        raise(SIGINT);
    }

    return exitstatus;
}

/* Funcion called from main thread upon receiving signal SIGTERM */
void
alterp(int sig) {
    ngSpice_Command("bg_halt");
}


/* Case insensitive str eq. */
/* Like strcasecmp( ) XXX */

int
cieq(register char *p, register char *s)
{
    while (*p) {
        if ((isupper(*p) ? tolower(*p) : *p) !=
            (isupper(*s) ? tolower(*s) : *s))
            return(false);
        p++;
        s++;
    }
    return (*s ? false : true);
}

}
