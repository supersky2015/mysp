#include "stdafx.h"
#include "test-sources.h"
#include <include/device.h>
#include <include/circuit.h>
#include <include/schema.h>
#include <common/common.h>

void test_ac_and_indicator()
{
	ngac ac("ac", 0, 1, 50);
	ngresistor r("r", 5);
	ngac_voltmeter vac("vac");
	ngac_ammeter iac("iac");
	ngground g;

	ngline l1(ac.p1, r.p1);
	ngline l6(r.p2, iac.p1);
	ngline l2(iac.p2, ac.p2);
	ngline l3(ac.p2, g.ground);
	ngline l4(vac.p1, r.p1);
	ngline l5(vac.p2, r.p2);

	schema sch;
	sch.AddDevices(&ac, &r, &g, &vac, &iac, 0);
	sch.AddLines(&l1, &l2, &l3, &l4, &l5, &l6, 0);

	circuit cir(&sch);
	cir.Tran("1s", "100u");
	cir.Wait();
	cir.Plot("v(1)");
	getchar();
}

void test_dc_current()
{
	ngdc_current dc("dc");
	ngresistor r("r", 5);
	ngground g;

	ngline l1(dc.pos, r.p1);
	ngline l2(r.p2, dc.neg);
	ngline l3(dc.neg, g.ground);

	schema sch;
	sch.AddDevices(&dc, &r, &g, 0);
	sch.AddLines(&l1, &l2, &l3, 0);

	circuit cir(&sch);
	int reg = 5;
	cir.Tran();
	do 
	{
		char key = getchar();
		switch (key)
		{
		case 'a':
			{
				cir.Halt();
				string cmd = format_string("alter rr=%d", reg++);
				cir.Do(cmd.c_str());
				cir.Resume();
			}
			break;
		case 'q':
			cir.Halt();
			break;
		}
		//cir.Resume();
		Sleep(100);
	} while (cir.IsRunning());
	string pcmd = format_string("plot %s", r.vec(0).c_str());
	cir.Do(pcmd.c_str());
	getchar();
}

void test_ac_current()
{
	ngac_current ac("ac", 0, 1, 1, 0, 60);
	ngresistor r("r", 5);
	ngac_current ac1("ac1", 0, 1, 1, 0, 0);
	ngresistor r1("r1", 5);
	ngground g;

	ngline l1(ac.pos, r.p1);
	ngline l2(r.p2, ac.neg);
	ngline l3(ac.neg, g.ground);
	ngline l4(ac1.pos, r1.p1);
	ngline l5(r1.p2, ac1.neg);
	ngline l6(ac1.neg, g.ground);

	schema sch;
	sch.AddDevices(&ac, &r, &g, &ac1, &r1, 0);
	sch.AddLines(&l1, &l2, &l3, &l4, &l5, &l6, 0);

	circuit cir(&sch);
	cir.Tran("3s", "10m");
	cir.Wait();
	string pcmd = format_string("plot %s %s", r.vec(0).c_str(), r1.vec(0).c_str());
	cir.Do(pcmd.c_str());
	getchar();
}


void test_vcvs()
{
	ngdc dc("dc");
	ngvcvs vcvs("vcvs", 2);
	ngresistor r("r1", 2.5);
	ngground g;
	ngvoltmeter v1("v1");
	ngvoltmeter v2("v2");
	ngammeter a1("a1");

	ngline l1(dc.pos, vcvs.VCVS_CPOS);
	ngline l2(dc.neg, vcvs.VCVS_CNEG);
	ngline l3(dc.neg, g.ground);
	ngline l4(vcvs.VCVS_POS, a1.A_POS);
	ngline l11(a1.A_NEG, r.p1);
	ngline l5(vcvs.VCVS_NEG, r.p2);
	ngline l6(vcvs.VCVS_NEG, g.ground);
	ngline l7(v1.V_POS, dc.pos);
	ngline l8(v1.V_NEG, dc.neg);
	ngline l9(v2.V_POS, r.p1);
	ngline l10(v2.V_NEG, r.p2);


	schema sch;
	sch.AddDevices(&dc, &vcvs, &g, &r, &v1, &v2, &a1, 0);
	sch.AddLines(&l1, &l2, &l3, &l4, &l5, &l6, &l7, &l8, &l9, &l10, &l11, 0);

	circuit cir(&sch);
	cir.Tran("1", "0.01");
	cir.Wait();
	string pcmd = format_string("plot %s %s", dc.vec(0).c_str(), r.vec(r.p1).c_str());
	cir.Do(pcmd.c_str());
	getchar();
}

void test_cccs()
{
	ngdc dc("dc", 5);
	ngresistor r1("r1", 10);
	ngcccs cccs("cccs", 2.0);
	ngresistor r2("r2", 2);
	ngground g;

	ngline l1(dc.pos, r1.p1);
	ngline l2(r1.p2, cccs.CCCS_CPOS);
	ngline l3(cccs.CCCS_CNEG, dc.neg);
	ngline l4(dc.neg, g.ground);
	ngline l5(cccs.CCCS_POS, r2.p1);
	ngline l6(cccs.CCCS_NEG, r2.p2);
	ngline l7(cccs.CCCS_NEG, g.ground);

	schema sch;
	sch.AddDevices(&dc, &cccs, &g, &r1, &r2, 0);
	sch.AddLines(&l1, &l2, &l3, &l4, &l5, &l6, &l7, 0);

	circuit cir(&sch);
	cir.Tran("1", "0.01");
	cir.Wait();
	//string pcmd = format_string("plot %s %s", dc.branch().c_str(), r2.vec(0).c_str());
	string pcmd = format_string("plot %s %s", cccs.branch("v", "v1").c_str(), r2.vec(r2.p1).c_str());
	cir.Do(pcmd.c_str());
	getchar();
}

void test_vccs()
{
	ngdc dc("dc");
	ngvccs vccs("vccs", 2);
	ngresistor r("r1", 1);
	ngground g;

	ngline l1(dc.pos, vccs.VCCS_CPOS);
	ngline l2(dc.neg, vccs.VCCS_CNEG);
	ngline l3(dc.neg, g.ground);
	ngline l4(vccs.VCCS_POS, r.p1);
	ngline l5(vccs.VCCS_NEG, r.p2);
	ngline l6(vccs.VCCS_NEG, g.ground);

	schema sch;
	sch.AddDevices(&dc, &vccs, &g, &r, 0);
	sch.AddLines(&l1, &l2, &l3, &l4, &l5, &l6, 0);

	circuit cir(&sch);
	cir.Tran("1", "0.01");
	cir.Wait();
	string pcmd = format_string("plot %s %s", dc.vec(dc.pos).c_str(), r.vec(r.p1).c_str());
	cir.Do(pcmd.c_str());
	getchar();
}

void test_ccvs()
{
	ngdc dc("dc", 5);
	ngccvs ccvs("ccvs", 2.0);
	ngresistor r1("r1", 10);
	ngresistor r2("r2", 2);
	ngground g;

	ngline l1(dc.pos, r1.p1);
	ngline l2(r1.p2, ccvs.CCVS_CPOS);
	ngline l3(ccvs.CCVS_CNEG, dc.neg);
	ngline l4(dc.neg, g.ground);
	ngline l5(ccvs.CCVS_POS, r2.p1);
	ngline l6(ccvs.CCVS_NEG, r2.p2);
	ngline l7(ccvs.CCVS_NEG, g.ground);

	schema sch;
	sch.AddDevices(&dc, &ccvs, &g, &r1, &r2, 0);
	sch.AddLines(&l1, &l2, &l3, &l4, &l5, &l6, &l7, 0);

	circuit cir(&sch);
	cir.Tran("1", "0.01");
	cir.Wait();
	//string pcmd = format_string("plot %s %s", dc.branch().c_str(), r2.vec(0).c_str());
	string pcmd = format_string("plot %s %s", ccvs.branch("v", "v1").c_str(), r2.vec(r2.p1).c_str());
	cir.Do(pcmd.c_str());
	getchar();
}
