#include "stdafx.h"
#include "test-device-basic.h"
#include <include/device.h>
#include <include/circuit.h>
#include <include/schema.h>

void test_basic_ngspst_pack()
{
	ngdc dc1("dc1");
	ngdc dc2("dc2");
	ngresistor r1("r1", 100);
	ngresistor r2("r2", 100);
	ngled led1("led1");
	ngled led2("led2");
	ngspst_pack spst2("spst2", 2);
	ngground gnd;

	ngline l1(dc1.pos, r1.p1);
	ngline l2(dc2.pos, r2.p1);
	ngline l3(r1.p2, led1.pos);
	ngline l4(r2.p2, led2.pos);
	ngline l5(led1.neg, spst2.p1);
	ngline l6(led2.neg, spst2.p3);
	ngline l7(spst2.p2, dc1.neg);
	ngline l8(spst2.p4, dc2.neg);
	ngline l9(dc1.neg, gnd.ground);
	ngline l10(dc2.neg, gnd.ground);

	schema sch("schema");
	sch.AddDevices(&dc1, &dc2, &r1, &r2, &led1, &led2, &spst2, &gnd, 0);
	sch.AddLines(&l1, &l2, &l3, &l4, &l5, &l6, &l7, &l8, &l9, &l10, 0);

	circuit cir(&sch);
	cir.Tran();
	do 
	{
		char key = getchar();
		switch (key)
		{
		case ' ':
			{
				string cmd = spst2.switchover();
				cir.Execute(cmd);
			}
			break;
		case 'q':
			cir.Halt();
			break;
		}
		//cir.Resume();
		Sleep(100);
	} while (cir.IsRunning());
}

void test_basic_ngspdt_pack()
{
	ngdc dc1("dc1");
	ngdc dc2("dc2");
	ngresistor r1("r1", 100);
	ngresistor r2("r2", 100);
	ngled led1("led1");
	ngled led2("led2");
	ngled led3("led3");
	ngled led4("led4");	
	ngspdt_pack spdt2("spdt2", 2);
	ngground gnd;

	ngline l1(dc1.pos, spdt2.p1_pole);
	ngline l2(spdt2.p1_throw1, led1.pos);
	ngline l3(spdt2.p1_throw2, led2.pos);
	ngline l4(led1.neg, r1.p1);
	ngline l5(led2.neg, r1.p1);
	ngline l6(r1.p2, dc1.neg);
	ngline l7(dc1.neg, gnd.ground);

	ngline la(dc2.pos, spdt2.p2_pole);
	ngline lb(spdt2.p2_throw1, led3.pos);
	ngline lc(spdt2.p2_throw2, led4.pos);
	ngline ld(led3.neg, r2.p1);
	ngline le(led4.neg, r2.p1);
	ngline lf(r2.p2, dc2.neg);
	ngline lg(dc2.neg, gnd.ground);

	schema sch;
	sch.AddDevices(&dc1, &dc2, &r1, &r2, &led1, &led2, &led3, &led4, &spdt2, &gnd, 0);
	sch.AddLines(&l1, &l2, &l3, &l4, &l5, &l6, &l7, &la, &lb, &lc, &ld, &le, &lf, &lg, 0);

	circuit cir(&sch);
	cir.Tran();
	do 
	{
		char key = getchar();
		switch (key)
		{
		case ' ':
			{
				string cmd = spdt2.switchover();
				cir.Execute(cmd);
			}
			break;
		case 'q':
			cir.Halt();
			break;
		}
		Sleep(100);
	} while (cir.IsRunning());
}

void test_ngspdt_pack2()
{
	ngdc dc("dc1", 4);
	ngresistor r1("r1", 1);
	ngresistor ra("ra", 1);
	ngresistor rb("rb", 3);
	ngammeter am("am");
	ngvoltmeter vm("vm");
	ngspdt_pack spdt2("spdt2", 2);
	ngground gnd;

	ngline l1(dc.pos, r1.p1);
	ngline l2(r1.p2, spdt2.p2_pole);
	ngline l3(spdt2.p2_throw1, ra.p1);
	ngline l4(spdt2.p2_throw2, rb.p1);
	ngline l5(ra.p2, spdt2.p1_throw1);
	ngline l6(rb.p2, spdt2.p1_throw2);
	ngline l7(spdt2.p1_pole, am.pos);
	ngline l8(am.neg, dc.neg);
	ngline l10(dc.neg, gnd.ground);
	ngline l11(vm.p1, spdt2.p2_pole);
	ngline l12(vm.p2, spdt2.p1_pole);

	schema sch;
	sch.set_print_sort(true);
	sch.AddDevices(&dc,&r1, &ra, &rb, &am, &vm, &spdt2, &gnd, 0);
	sch.AddLines(&l1, &l2, &l3, &l4, &l5, &l6, &l7, &l8, &l10, &l11, &l12, 0);

	circuit cir(&sch);
	cir.Tran();
	do 
	{
		char key = getchar();
		switch (key)
		{
		case ' ':
			{
				string cmd = spdt2.switchover();
				cir.Execute(cmd);
			}
			break;
		case 'q':
			cir.Halt();
			break;
		}
		Sleep(100);
	} while (cir.IsRunning());
}
