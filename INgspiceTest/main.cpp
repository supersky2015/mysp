#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#include <stdbool.h>
#include <pthread.h>
#else
#define bool int
#define true 1
#define false 0
#define strdup _strdup
#endif
#include <signal.h>
#if defined(__MINGW32__) || defined(_MSC_VER)
#include <windows.h>  // for Sleep
#else
#include <unistd.h>
#include <ctype.h>
#endif
#include <ngspice/sharedspice.h>
#include "common/common.h"

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


bool no_bg = true;
int vecgetnumber = 0;
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

void init()
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
}

int main4(int argc, _TCHAR* argv[])
{
	init();

    int ret, i;
    char *curplot, *vecname;
    char ** circarray;
    char **vecarray;
    
    ret = ngSpice_Init(ng_getchar, ng_getstat, ng_exit,  ng_data, ng_initdata, ng_thread_runs, NULL);

    printf("Init thread returned: %d\n", ret);

    testnumber = 3;
    printf("\n**  Test no %d with flag for stopping background thread  **\n\n", testnumber);
   /* create an RC circuit, uase transient simulation */
    circarray = (char**)malloc(sizeof(char*) * 7);
    circarray[0] = strdup("test array");
    circarray[1] = strdup("V1 1 0 1");
    circarray[2] = strdup("R1 1 2 1");
    circarray[3] = strdup("C1 2 0 1 ic=0");
    circarray[4] = strdup(".tran 100u 5 uic");
    circarray[5] = strdup(".end");
    circarray[6] = NULL;

    has_break = false;

	timer t;
    ret = ngSpice_Circ(circarray);
    ret = ngSpice_Command("bg_run");

    for(i = 0; i < 6; i++)
        free(circarray[i]);
    free(circarray);

    /* wait until simulation stops */
    for (;;) {
        Sleep (100);
#if 1
        if (has_break) {
            ret = ngSpice_Command("bg_halt");
            //ret = ngSpice_Command("listing");
            //ret = ngSpice_Command("alter c1=2");
            //ret = ngSpice_Command("bg_resume");
        }
#endif
        if (no_bg)
            break;
    }

    /* wait until simulation finishes */
    //for (;;) {
    //    Sleep (100);
    //    if (no_bg)
    //        break;
    //}
	printf("time used : %fs\n", t.Escape());
	ret = ngSpice_Command("bg_run");
	for (;;) {
		Sleep (15);
		if (no_bg)
			break;
	}
	printf("time used : %fs\n", t.Escape());

    ret = ngSpice_Command("write test3.raw V(2)");

	printf("time used : %fs\n", t.Escape());

	getchar();
    return 0;

    return ret;
}

int
ng_getchar(char* outputreturn, int ident, void* userdata)
{
   // printf("1: %s\n", outputreturn);
    return 0;
}


int
ng_getstat(char* outputreturn, int ident, void* userdata)
{
    //printf("2: %s\n", outputreturn);
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
#if 1
	static int print = -1;
	int p = vdata->vecsa[vecgetnumber]->creal*100;
	if (print != p)	{
		print = p;
		printf("%08x, %d, %d, %f\n", vdata, numvecs, ident, vdata->vecsa[vecgetnumber]->creal);
	}
    int *ret;

    v2dat = vdata->vecsa[vecgetnumber]->creal;
    if (!has_break && (v2dat > 0.5)) {
//        printf("Data V(2) value: %f\n", v2dat);
    /* using signal SIGTERM by sending to main thread, alterp() then is run from the main thread,
      (not on Windows though!)  */
        has_break = true;
        printf("Pause requested, setpoint reached\n");
    /* leave bg thread for a while to allow halting it from main */
        Sleep (100);
//        ret = ((int * (*)(char*)) ngSpice_Command_handle)("bg_halt");
    }
#endif
    return 0;
}


/* Callback function called from bg thread in ngspice once upon intialization
   of the simulation vectors)*/
int
ng_initdata(pvecinfoall intdata, int ident, void* userdata)
{
	printf("< %s, %s, %s, %s >\n", intdata->type, intdata->title, intdata->name, intdata->date);
	int i;
    int vn = intdata->veccount;
    for (i = 0; i < vn; i++) {
        printf("Vector: %s\n", intdata->vecs[i]->vecname);
        /* find the location of V(2) */
        if (cieq(intdata->vecs[i]->vecname, "V(2)"))
            vecgetnumber = i;
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
