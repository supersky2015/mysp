#include "stdafx.h"

#include <include/schema.h>
#include <include/circuit.h>

void test_schema_rc()
{
	ngdc dc("dc1", 5);
	ngresistor r("r1", 5);
	ngcapacitor c("c1", 0.2);
	ngground gnd;

	ngline line1(dc.pos, r.p1);
	ngline line2(r.p2, c.p1);
	ngline line3(c.p2, dc.neg);
	ngline line4(dc.pos, gnd.ground);

	schema sch("design1");
	sch.AddDevice(&dc);
	sch.AddDevice(&r);
	sch.AddDevice(&c);
	sch.AddDevice(&gnd);

	sch.AddLine(&line1);
	sch.AddLine(&line2);
	sch.AddLine(&line3);
	sch.AddLine(&line4);

	vector<string> netlist = sch.GetNetlist();

	for (auto it = netlist.begin(); it != netlist.end(); it++){
		printf("%s\n", it->c_str());
	};

	//getchar();
}

void test_schema_multiline()
{
/*
 *A Berkeley SPICE3 compatible circuit
 vcc vcc 0 12.0
 vin 1 0 0.0 ac 1.0 sin(0 1 1k)
 ccouple 1 base 10uF
 rbias1 vcc base 100k
 rbias2 base 0 24k
 q1 coll base emit generic
 rcollector vcc coll 3.9k
 remitter emit 0 1k
 *
 .model generic npn
 *
 */
	ngdc vcc("vcc", 12);
	ngac vin("vin", 0, 1, 1000, 0);
	ngcapacitor ccouple("ccouple", 10e-9);
	ngresistor rbias1("bias1", 10e5);
	ngresistor rbias2("bias2", 2.4e4);
	ngbjt_npn q1("q1");
	ngresistor rcoll("rcoll", 3.9e3);
	ngresistor remit("remit", 1000);
	ngground gnd;

	ngline line0(vcc.neg, gnd.p1);
	ngline line1(vcc.neg, vin.p2);
	ngline line2(vcc.neg, rbias2.p2);
	ngline line3(vcc.neg, remit.p2);
	ngline line4(vcc.pos, rbias1.p1);
	ngline line5(vcc.pos, rcoll.p1);
	ngline line6(vin.p1, ccouple.p1);
	ngline line7(ccouple.p2, rbias1.p2);
	ngline line8(ccouple.p2, rbias2.p1);
	ngline line9(ccouple.p2, q1.p2);
	ngline line10(q1.p1, rcoll.p2);
	ngline line11(q1.p3, remit.p1);

 	schema sch("design1");
 	sch.AddDevice(&vcc);
	sch.AddDevice(&vin);
 	sch.AddDevice(&ccouple);
 	sch.AddDevice(&rbias1);
 	sch.AddDevice(&rbias2);
	sch.AddDevice(&q1);
	sch.AddDevice(&rcoll);
	sch.AddDevice(&remit);
	sch.AddDevice(&gnd);
 
	sch.AddLine(&line0);
 	sch.AddLine(&line1);
 	sch.AddLine(&line2);
 	sch.AddLine(&line3);
 	sch.AddLine(&line4);
	sch.AddLine(&line5);
	sch.AddLine(&line6);
	sch.AddLine(&line7);
	sch.AddLine(&line8);
	sch.AddLine(&line9);
	sch.AddLine(&line10);
	sch.AddLine(&line11);

 	vector<string> netlist = sch.GetNetlist();

	for (auto it = netlist.begin(); it != netlist.end(); it++){
		printf("%s\n", it->c_str());
	};
}