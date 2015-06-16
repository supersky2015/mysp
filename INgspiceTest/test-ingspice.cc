// INgspiceTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h> 
#include <include/circuit.h>
#include <common/common.h>

int run_parallel()
{
	vector<string> netlist1, netlist2;
	netlist1.push_back("test array");
	netlist1.push_back("V1 1 0 1");
	netlist1.push_back("R1 1 2 1");
	netlist1.push_back("C1 2 2 1 ic=0");
	netlist1.push_back(".tran 100u 1 uic");
	netlist1.push_back(".end");

	netlist2.push_back("test array");
	netlist2.push_back("V1 1 0 1");
	netlist2.push_back("R1 1 2 1");
	netlist2.push_back("C1 2 0 1 ic=0");
	netlist2.push_back(".tran 100u 1 uic");
	netlist2.push_back(".end");

	timer t;
	ngspice ng1, ng2;
	ng1.LoadNetlist(netlist1);
	ng2.LoadNetlist(netlist2);

	ng1.Run();
	ng2.Run();

	do {
		Sleep(200);
	} while (ng1.IsRunning() || ng2.IsRunning());
	
	ng1.Do("rusage");
	ng2.Do("rusage");
	ng1.Do("listing");
	ng2.Do("listing");

	ng1.GetAllPlotsVecs();

	ng2.GetAllPlotsVecs();

	getchar();

	return 0;
}

int load_error()
{
	ngspice ng;
	if (!ng.LoadCircuit("3p_load_error.cir")) {
		printf("%s", ng.ErrorMsgLoading().c_str());
		return -1;
	}
	ng.Run();

	do 
	{
		Sleep(200);
	} while (ng.IsRunning());

	ng.GetAllPlotsVecs();
	getchar();
	return 0;
}

int load_error2()
{
	vector<string> netlist;
	netlist.push_back("test array");
	netlist.push_back("vin2 1 0 10.0 ac 1.0 sin(0 1 1 )");
	netlist.push_back("vin2 2 0 10.0 ac 1.0 sin(0 1 1 -0.33333333)");
	netlist.push_back("vin2 2 0 10.0 ac 1.0 sin(0 1 1 -0.66666666)");
	netlist.push_back("R1 1 0 2");
	netlist.push_back("r2 2 0 2");
	netlist.push_back(".tran 100u 1 uic");
	netlist.push_back(".end");

	ngspice ng;
	if (!ng.LoadNetlist(netlist)) {
		printf("%s", ng.ErrorMsgLoading().c_str());
		getchar();
		return -1;
	}
	ng.Run();

	do 
	{
		Sleep(200);
	} while (ng.IsRunning());

	ng.GetAllPlotsVecs();
	getchar();
	return 0;
}

int run_error()
{
	ngspice ng;
	if (!ng.LoadCircuit("3p_run_error.cir")) {
		printf("%s", ng.ErrorMsgLoading().c_str());
		return -1;
	}

	ng.Run();

	do 
	{
		Sleep(200);
	} while (ng.IsRunning());

	string runerr = ng.ErrorMsgLoading();
	printf("%s\n", runerr);

	ng.GetAllPlotsVecs();
	getchar();
	return 0;
}

int test_rc()
{
	vector<string> netlist;
	netlist.push_back("test array");
	netlist.push_back("V1 1 0 1");
	netlist.push_back("R1 1 2 1");
	netlist.push_back("C1 2 2 1 ic=0");
	netlist.push_back(".tran 100u 1 uic");
	netlist.push_back(".end");

	timer t;
	ngspice ng;
	bool success = ng.LoadNetlist(netlist);
	//bool success = ng.LoadCircuit("C:\\spice\\bin\\rc.cir");
	//bool success = ng.LoadCircuit("E:\\workspace\\NGSpice\\A Wrap of Ngspice\\INgspice\\INgspiceTest\\adder_mos.cir");
	//bool success = ng.LoadCircuit("C:\\spice\\bin\\adder_mos.cir");
	ng.Do("listing");
	success &= ng.Run();
	do {
		Sleep(200);
	}while (success && ng.IsRunning());

	/*
	dvec* dv1 = (dvec*)(ng.m_plots[0].pvecs[0]->pdvec);
	dvec* dv2 = (dvec*)(ng.m_plots[0].pvecs[1]->pdvec);
	dvec* dv3 = (dvec*)(ng.m_plots[0].pvecs[2]->pdvec);
	dvec* dv4 = (dvec*)(ng.m_plots[0].pvecs[3]->pdvec);
	vector<double> v1, v2, v3, v4;
	for (int i = 0; i < dv1->v_length; i++)	{
		v1.push_back(dv1->v_realdata[i]);
		v2.push_back(dv2->v_realdata[i]);
		v3.push_back(dv3->v_realdata[i]);
		v4.push_back(dv4->v_realdata[i]);
	}
	*/
	printf("time used : %fs\n", t.Escape());
#if 0
	ng.Halt();
	success &= ng.Run();
	do {
		Sleep(100);
	}while (success && ng.IsRunning());
	printf("time used : %fs\n", t.Escape());
#endif

	
#if 0
	vector<double> v1, v2, v3, v4;
	for (int i = 0; i < ng.GetPlot().vecsize; i++)	{
		v1.push_back(ng.GetPlot().vecs[0].values[i]);
		v2.push_back(ng.GetPlot().vecs[1].values[i]);
		v3.push_back(ng.GetPlot().vecs[2].values[i]);
		v4.push_back(ng.GetPlot().vecs[3].values[i]);
	}
#endif	
	ng.GetAllPlotsVecs();
	getchar();
	return 0;
}

int test_3p_effective_voltage()
{
#if 0
	vector<string> netlist;
	netlist.push_back("test array");
	netlist.push_back("vin1 1 0 10.0 ac 1.0 sin(0 1 10 )");
	netlist.push_back("vin2 2 0 10.0 ac 1.0 sin(0 1 10 -0.03333333333)");
	netlist.push_back("vin3 3 0 10.0 ac 1.0 sin(0 1 10 -0.06666666666)");
	netlist.push_back("R1 1 0 2");
	netlist.push_back("r2 2 0 2");
	netlist.push_back("r3 3 0 2");
	netlist.push_back(".tran 100u 1000 uic");
	netlist.push_back(".end");

	ngspice ng;
	if (!ng.LoadNetlist(netlist))
		return -1;

	do 
	{
		Sleep(2000);
		ng.Halt();
		ng.Do("meas tran rms_v1 RMS v(1) from=0m to=1000");
		ng.Resume();
		Sleep(2000);
	} while (ng.IsRunning());
#else
	vector<string> netlist;
	netlist.push_back("test array");
	netlist.push_back("vin1 1 0 10.0 ac 1.0 sin(0 1 10 )");
	netlist.push_back("vin2 2 0 10.0 ac 1.0 sin(0 1 10 -0.03333333333)");
	netlist.push_back("vin3 3 0 10.0 ac 1.0 sin(0 1 10 -0.06666666666)");
	netlist.push_back("R1 1 0 2");
	netlist.push_back("r2 2 0 2");
	netlist.push_back("r3 3 0 2");
	netlist.push_back(".tran 100u 10 uic");
	netlist.push_back(".end");

	ngspice ng;
	if (!ng.LoadNetlist(netlist))
		return -1;

	ng.Run();
	do 
	{
		Sleep(100);
	} while (ng.IsRunning());
	
	ng.Do("meas tran rms_v1 RMS v(1) from=0m to=1000");

#endif

	ng.GetAllPlotsVecs();
	getchar();
	return 0;
}

int test_xspice()
{
	ngspice ng;
	if (!ng.LoadCircuit("xspice_c2.cir"))
		return -1;

	ng.Run();
	do 
	{
		Sleep(100);
	} while (ng.IsRunning());

	ng.GetAllPlotsVecs();
	ng.Do("print aout");
	ng.Do("assciiplot aout");
	getchar();
	return 0;
}
