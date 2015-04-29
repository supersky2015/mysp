#include "stdafx.h"

#include <include/schema.h>
#include <include/circuit.h>

void test_schema_rc()
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

	vector<string> netlist = sch.GetNetlist();

	for (auto it = netlist.begin(); it != netlist.end(); it++){
		printf("%s\n", it->c_str());
	};

	getchar();
}
	/*
	A Berkeley SPICE3 compatible circuit
	*
	*This circuit contains only Berkeley SPICE3 components.
	*
	*The circuit is an ACcoupled transistor amplifier with
	*a sine wave input at node"1",a gain of approximately-3.9,
	*and output on node "coll".
	*
	.tran 1e-5 2e-3
	*
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
	.end
	 */
void test_schema_multiline()
{
	ngdc vcc("vcc", 12);
	ngac vin("vin", 0, 1, 1000, 0);
	ngcapacitor ccouple("ccouple", 10e-9);
	ngresistor rbias1("bias1", 2.4e4);
	ngresistor r("r1", 5);
	ngcapacitor c("c1", 0.2);
	ngground gnd;

// 	ngline line1(dc[0], r[0]);
// 	ngline line2(r[1], c[0]);
// 	ngline line3(c[1], dc[1]);
// 	ngline line4(dc[0], gnd[0]);
// 
// 	schema sch("design1");
// 	sch.AddDevice(&dc);
// 	sch.AddDevice(&r);
// 	sch.AddDevice(&c);
// 	sch.AddDevice(&gnd);
// 
// 	sch.AddLine(&line1);
// 	sch.AddLine(&line2);
// 	sch.AddLine(&line3);
// 	sch.AddLine(&line4);

// 	vector<string> netlist = sch.GetNetlist();

	//for (auto it = netlist.begin(); it != netlist.end(); it++){
	//	printf("%s\n", it->c_str());
	//};
}