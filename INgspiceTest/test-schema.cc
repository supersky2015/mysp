#include "stdafx.h"

#include <include/schema.h>
#include <include/circuit.h>
#include <common/common.h>

void debug_line( ngline* line )
{
	output_debug_message(" <ngline [%s %s]	<==>	[%s %s]		%d/>", line->c1.name.c_str(), line->c1.pin.c_str(), line->c2.name.c_str(), line->c2.pin.c_str(), line->order);
}

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

void test_schema_connection()
{
	ngresistor r1("r1", 1);
	ngresistor r2("r2", 1);
	ngresistor r3("r3", 1);
	ngresistor r4("r4", 1);
	ngresistor r5("r5", 1);
	ngresistor r6("r6", 1);
	ngresistor r7("r7", 1);
	ngresistor r8("r8", 1);
	ngresistor r9("r9", 1);

	ngline* lines[] = {
#if 0//case 1
		// l1 <==> l3;	l3 <==> l5
		new ngline(r1.p1, r2.p1),	//l1
		new ngline(r2.p2, r3.p1),	//l2
		new ngline(r2.p1, r4.p1),	//l3
		new ngline(r4.p2, r5.p1),	//l4
		new ngline(r4.p1, r6.p1),	//l5
		new ngline(r7.p1, r8.p1),	//l6
#endif

#if 0//case 2
		// l1 <==> l5; l3 <==> l5
		new ngline(r1.p1, r1.p2),
		new ngline(r2.p1, r2.p2),
		new ngline(r3.p1, r3.p2),
		new ngline(r4.p1, r4.p2),
		new ngline(r1.p2, r3.p1),
		new ngline(r6.p1, r6.p2),
#endif

#if 1//case 3
		// l1 <==> l5; l2 <==> l3; l3 <==> l5
		new ngline(r1.p1, r1.p2),
		new ngline(r2.p1, r2.p2),
		new ngline(r2.p1, r3.p2),
		new ngline(r4.p1, r4.p2),
		new ngline(r1.p1, r3.p2),
		new ngline(r6.p1, r6.p2),
#endif
	};

	schema sch("design1");
	sch.set_print_sort(true);
	size_t count = sizeof(lines)/sizeof(ngline*);
	for (size_t i = 0; i < sizeof(lines)/sizeof(ngline*); i++)
		sch.AddLine(lines[i]);

	vector<string> netlist = sch.GetNetlist();
	for (size_t i = 0; i < sizeof(lines)/sizeof(ngline*); i++)
		debug_line(lines[i]);

	for (size_t i = 0; i < sizeof(lines)/sizeof(ngline*); i++)
		delete lines[i];
}

