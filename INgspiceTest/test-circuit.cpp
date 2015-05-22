#include "stdafx.h"
#include <windows.h>
#include <include/schema.h>
#include <include/circuit.h>
#include <include/ngspice.h>

void test_restart()
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
	cir.Tran("1000");

	do 
	{
		Sleep(200);
		char ch = getchar();
		switch (ch)
		{
		case 'r':
			cir.Restart();
			Sleep(200);
			break;
		default:
			break;
		}
	} while (cir.IsRunning());
}

void test_circuit_rc()
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

void test_flash_led()
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
	
	getchar();
}

void test_circuit_parallel()
{
	//circuit 1
	ngac AC("ac1", 0, 5, 1);
	ngresistor R("r1", 370);
	ngled LED("LED1", 7e-3);
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

void test_switch_by_resistor()
{
	ngdc dc("dc", 10);
	ngresistor r("r1", 370);
	ngled led("led1", 5e-3);
	ngswitch sw("sw", ngswitch::off);
	ngground gnd;

	ngline l0(dc.neg, gnd.p1);
	ngline l1(dc.pos, r.p1);
	ngline l2(r.p2, led.p1);
	ngline l3(led.p2, sw.p1);
	ngline l4(sw.p2, dc.neg);

	schema sch;
	sch.AddDevices(&dc, &r, &led, &gnd, &sw, 0);
	sch.AddLines(&l0, &l1, &l2, &l3, &l4, 0);

	circuit cir(&sch);
	cir.Tran("1t", "1m");

	do 
	{
		Sleep(200);
		char ch = getchar();
		switch (ch)
		{
		case 'a':
			cir.SwitchOver(&sw);
			Sleep(200);
			break;
		case 'q':
			cir.Halt();
		default:
			break;
		};
	} while (cir.IsRunning());

}

void test_switch_by_csw_inter()
{
	ngdc dc("dc1", 5);
	ngspst spst("spst");
	ngresistor r("1", 5);
	ngground gnd;

	ngline l1(dc.pos, spst.p1);
	ngline l2(spst.p2, r.p1);
	ngline l3(r.p2, dc.neg);
	ngline l0(dc.neg, gnd.ground);

	schema sch;
	sch.AddDevices(&dc, &spst, &r, &gnd, 0);
	sch.AddLines(&l1, &l2, &l3, &l0, 0);

	circuit cir(&sch);

#if 1//ok now
	vector<string> netlist = sch.GetNetlist();
	// tran with spst disconnected
	printf("-----spst disconnected---------\n");
	cir.LoadNetlist(netlist);
	cir.Do("listing");
	cir.Do("tran 1m 10 uic");


	// tran with spst connected
	printf("-----spst connected---------\n");
	string sw = spst.switchover();
	cir.Do(sw.c_str());
	cir.Do("listing");
	cir.Do("tran 1m 10 uic");

#endif
}

void test_switch_by_csw()
{
	ngdc dc("dc1", 5);
	ngspst spst("spst", ngspst::on);
	ngresistor r("1", 5);
	ngled led("led");
	ngground gnd;

	ngline l1(dc.pos, spst.p1);
	ngline l2(spst.p2, r.p1);
	ngline l3(r.p2, led.pos);
	ngline l4(led.neg, dc.neg);
	ngline l0(dc.neg, gnd.ground);

	schema sch;
	sch.AddDevices(&dc, &spst, &r, &gnd, &led, 0);
	sch.AddLines(&l1, &l2, &l3, &l0, &l4, 0);

	circuit cir(&sch);

#if 0//not work
	// tran with spst disconnected
	cir.Tran("10", "1m");
	do 
	{
		Sleep(100);
	} while (cir.IsRunning());
	//cir.Stop();

	// tran with spst connected, however it's still disconnected
	string sw = spst.switchover();
	cir.Do(sw);
	cir.Tran("10", "1m");
	do 
	{
		Sleep(100);
	} while (cir.IsRunning());
#endif


	// run with event input to switch spst
	cir.Tran("1t", "1m", 0);
	do 
	{
		Sleep(200);
		char ch = getchar();
		switch (ch)
		{
		case 'a':
			cir.SwitchOver(&spst);
			Sleep(200);
			break;
		case 'q':
			cir.Halt();
		default:
			break;
		};
	} while (cir.IsRunning());
}

void test_spdt()
{
	//创建所需元器件
	ngdc dc("dc1", 5);
	ngspdt spdt("spdt", ngspdt::status_throw1);
	ngresistor r1("1", 5);
	ngresistor r2("2", 5);
	ngled led1("led1");
	ngled led2("led2");
	ngground gnd;

	//创建接线，连接各元器件
	ngline l1(dc.pos, spdt.pole);
	ngline l2(spdt.throw1, r1.p1);
	ngline l3(spdt.throw2, r2.p1);
	ngline l4(r1.p2, led1.pos);
	ngline l5(r2.p2, led2.pos);
	ngline l6(led1.neg, dc.neg);
	ngline l7(led2.neg, dc.neg);
	ngline l0(dc.neg, gnd.ground);

	//创建电路图，添加元器件、接线到电路图
	schema sch;
	sch.AddDevices(&dc, &spdt, &r1, &r2, &led1, &led2, &gnd, 0);
	sch.AddLines(&l1, &l2, &l3, &l4, &l5, &l6, &l7, &l0, 0);

	//创建仿真对象，添加电路图，并开始暂态仿真
	circuit cir(&sch);
	cir.Tran("1t", "1m", 0);
	do 
	{
		Sleep(200);
		char ch = getchar();
		switch (ch)
		{
		case 'a':
			cir.SwitchOver(&spdt);
			Sleep(200);
			break;
		case 'q':
			cir.Halt();
		default:
			break;
		};
	} while (cir.IsRunning()); //主程序线程，类似windows UI消息循环
}

void test_circuit_rc_tran()
{
	ngdc dc("dc1", 5);
	ngspdt spdt("spdt");
	ngresistor r("r1", 5);
	ngcapacitor c("c1", 0.2);
	ngground gnd;

	ngline line1(dc.pos, spdt.throw1);
	ngline line2(spdt.pole, r.p1);
	ngline line3(r.p2, c.p1);
	ngline line4(c.p2, dc.neg);
	ngline line5(spdt.throw2, gnd.ground);
	ngline line0(dc.neg, gnd.ground);

	schema sch;
	sch.AddDevices(&dc, &spdt, &r, &c, &gnd, 0);
	sch.AddLines(&line1, &line2, &line3, &line4, &line5, &line0, 0);


	circuit cir(&sch);
	cir.Tran("1t", "100u");

	do 
	{
		Sleep(200);
		char ch = getchar();
		switch (ch)
		{
		case 'a':
			// switchover to charge or discharge
			cir.SwitchOver(&spdt);
			Sleep(200);
			break;
		case 'q':
			cir.Halt();
		default:
			break;
		};
	} while (cir.IsRunning());

	cir.Do("plot all");
}

/* same effect to test_rc_charge_discharge()
title rc charge discharge
.global gnd
vdc1 1 0 dc 5.000e+000
xspdt 1 2 0 spdt
rr1 2 3 5.000e+000
cc1 3 0 2.000e-001

.subckt spdt 1 2 3 params: vstatus=0 ron=1e-8 roff=1e30
r1 0 6 20
v1 6 0 dc {vstatus}
w0 2 1 v1 nc_contact
w1 2 3 v1 no_contact
.model no_contact csw (it=0.05 ih=0.025 ron={ron} roff={roff})
.model nc_contact csw (it=0.05 ih=0.025 ron={roff} roff={ron})
.ends

.control
stop when time = 5s
tran 100u 10s uic
alter v.xspdt.v1=-2
resume
plot all
.endc
*/
void test_rc_charge_discharge()
{
	ngdc dc("dc1", 5);
	ngspdt spdt("spdt");
	ngresistor r("r1", 5);
	ngcapacitor c("c1", 0.2);
	ngground gnd;

	ngline line1(dc.pos, spdt.throw1);
	ngline line2(spdt.pole, r.p1);
	ngline line3(r.p2, c.p1);
	ngline line4(c.p2, dc.neg);
	ngline line5(spdt.throw2, gnd.ground);
	ngline line0(dc.neg, gnd.ground);

	schema sch;
	sch.AddDevices(&dc, &spdt, &r, &c, &gnd, 0);
	sch.AddLines(&line1, &line2, &line3, &line4, &line5, &line0, 0);

	circuit cir(&sch);
	// transient analysis last 10 seconds
	cir.Tran("10", "1m");
	do 
	{
		Sleep(100);
		//charge 5 seconds. when time passes 5 seconds, start to discharge.
		static bool done = false;
		if (cir.CurrentValue("time") >= 5 && !done)
		{
			cir.SwitchOver(&spdt);
			Sleep(200);
			done = true;
		}
	} while (cir.IsRunning());
	cir.Do("plot all");
	getchar();
}
