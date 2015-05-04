#include "stdafx.h"
#include <windows.h>
#include <include/schema.h>
#include <include/circuit.h>

void test_circuit1()
{
	ngdc dc("dc1", 5);
	ngresistor r("r1", 5);
	ngcapacitor c("c1", 0.2);
	ngground gnd;

	ngline line1(dc[0], r[0]);
	ngline line2(r[1], c[0]);
	ngline line3(c[1], dc[1]);
	ngline line4(dc[0], gnd[0]);

	schema sch("design1");
	sch.AddDevice(&dc);
	sch.AddDevice(&r);
	sch.AddDevice(&c);
	sch.AddDevice(&gnd);

	sch.AddLine(&line1);
	sch.AddLine(&line2);
	sch.AddLine(&line3);
	sch.AddLine(&line4);

	circuit cir(&sch);
	cir.Tran("1s");
	
	do 
	{
		Sleep(200);
	} while (cir.IsRunning());
}

void test_circuit_led()
{
	ngac ac("ac1", 0, 5, 1);
	ngresistor r("r1", 370);
	ngled led("led1", 5e-3);
	ngground gnd;

	ngline l0(ac.p1, gnd.p1);
	ngline l1(ac.p2, r.p1);
	ngline l2(r.p2, led.p1);
	ngline l3(led.p2, ac.p1);

	schema sch;
	sch.AddDevices(&ac, &r, &led, &gnd, 0);
	sch.AddLines(&l0, &l1, &l2, &l3, 0);

	circuit cir(&sch);
	cir.Tran("2", "10m");

	do 
	{
		Sleep(200);
	} while (cir.IsRunning());
	//sch.AddLine()
}

void test_circuit_parallel()
{
	//circuit 1
	ngac AC("ac1", 0, 5, 1);
	ngresistor R("r1", 370);
	ngled LED("LED1", 5e-3);
	ngground GND;

	ngline L0(AC.p1, GND.p1);
	ngline L1(AC.p2, R.p1);
	ngline L2(R.p2, LED.p1);
	ngline L3(LED.p2, AC.p1);

	schema SCH;
	SCH.AddDevices(&AC, &R, &LED, &GND, 0);
	SCH.AddLines(&L0, &L1, &L2, &L3, 0);

	circuit CIR(&SCH);
	CIR.Tran("20", "1m");

	//circuit 2
	ngac ac("ac1", 0, 5, 1);
	ngresistor r("r1", 370);
	ngled led("led1", 5e-3);
	ngground gnd;

	ngline l0(ac.p1, gnd.p1);
	ngline l1(ac.p2, r.p1);
	ngline l2(r.p2, led.p1);
	ngline l3(led.p2, ac.p1);

	schema sch;
	sch.AddDevices(&ac, &r, &led, &gnd, 0);
	sch.AddLines(&l0, &l1, &l2, &l3, 0);

	circuit cir(&sch);
	cir.Tran("20", "1m");

	do 
	{
		Sleep(200);
	} while (CIR.IsRunning() || cir.IsRunning());
}