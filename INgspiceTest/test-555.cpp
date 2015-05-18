#include "stdafx.h"
#include <windows.h>
#include <include/device.h>
#include <include/circuit.h>
#include <include/schema.h>
#include <common/common.h>

void test_555_monostable()
{
//uncomment to test lmc555 with pin CON open
//#define OPEN_TM_CON
#ifndef OPEN_TM_CON
	ngcapacitor c2("c2", 1e-8);
#endif

	ngdc dc("dc", 5);
	ngpluse pulse("pulse", 5, 0, 2e-3, 2e-2, 1e-9, 1e-9);
	ngresistor r("rcharge", 1000);
	ngresistor rload("rload", 500);
	ngcapacitor c1("c1", 1e-5);
	ngtimer t("u1");
	ngground gnd("gnd");

	ngline l1(dc.pos, t.TM_VCC);
	ngline l2(dc.pos, t.TM_RST);
	ngline l3(dc.pos, r.p1);
	ngline l4(r.p2, c1.p1);
	ngline l5(c1.p2, gnd.ground);
	ngline l6(pulse.pos, t.TM_TRI);
	ngline l7(pulse.neg, gnd.ground);
	ngline l8(t.TM_THR, r.p2);
	ngline l9(t.TM_DIS, r.p2);
	ngline l10(t.TM_GND, gnd.ground);
	ngline l11(t.TM_OUT, rload.p1);
	ngline l12(rload.p2, gnd.ground);
	ngline l13(dc.neg, gnd.ground);
#ifndef OPEN_TM_CON
	ngline l14(c2.p1, t.TM_CON);
	ngline l15(c2.p2, gnd.ground);
#endif
	
	schema sch;
#ifndef OPEN_TM_CON
	sch.AddDevices(&dc, &pulse, &r, &rload, &c1, &t, &gnd, &c2, 0);
	sch.AddLines(&l1, &l2, &l3, &l4, &l5, &l6, &l7, &l8, &l9, &l10, &l11, &l12, &l13, &l14, &l15, 0);
#else
	sch.AddDevices(&dc, &pulse, &r, &rload, &c1, &t, &gnd, 0);
	sch.AddLines(&l1, &l2, &l3, &l4, &l5, &l6, &l7, &l8, &l9, &l10, &l11, &l12, &l13, 0);
#endif

	circuit cir(&sch);
	cir.Tran("0.1", "100u");
	do
	{
		Sleep(100);
	} while (cir.IsRunning());
	//string cmd = format_string("plot %s %s %s", pulse.vec(0).c_str(), c1.vec(0).c_str(), t.vec(t.TM_OUT).c_str());
	string cmd = format_string("plot %s %s %s", pulse.vec(0).c_str(), c1.vec(0).c_str(), rload.vec(0).c_str());
	cir.Do(cmd);
	getchar();
}


