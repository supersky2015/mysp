#include "stdafx.h"
#include "test-8-way-responder.h"
#include <include/circuit.h>
#include <include/device.h>
#include <include/schema.h>


void test_7seg()
{
	ngdc dc1("dc1"); 
	ngdc dc2("dc2");
	ngdc dc3("dc3");
	ngdc dc4("dc4");
	ngdc dc5("dc5");
	ngdc dc6("dc6");
	ngdc dc7("dc7");
	
	ngspst s1("s1");
	ngspst s2("s2");
	ngspst s3("s3");
	ngspst s4("s4");
	ngspst s5("s5");
	ngspst s6("s6");
	ngspst s7("s7");
	
	ngseven_seg ss("ss");
	ngground gnd;

	ngline l1(dc1.pos, s1.p1);
	ngline l2(dc2.pos, s2.p1);
	ngline l3(dc3.pos, s3.p1);
	ngline l4(dc4.pos, s4.p1);
	ngline l5(dc5.pos, s5.p1);
	ngline l6(dc6.pos, s6.p1);
	ngline l7(dc7.pos, s7.p1);

	ngline l01(ss.SS_A, s1.p2);
	ngline l02(ss.SS_B, s2.p2);
	ngline l03(ss.SS_C, s3.p2);
	ngline l04(ss.SS_D, s4.p2);
	ngline l05(ss.SS_E, s5.p2);
	ngline l06(ss.SS_F, s6.p2);
	ngline l07(ss.SS_G, s7.p2);
	ngline l08(ss.SS_COM, gnd.ground);

	ngline l11(dc1.neg, gnd.ground);
	ngline l22(dc2.neg, gnd.ground);
	ngline l33(dc3.neg, gnd.ground);
	ngline l44(dc4.neg, gnd.ground);
	ngline l55(dc5.neg, gnd.ground);
	ngline l66(dc6.neg, gnd.ground);
	ngline l77(dc7.neg, gnd.ground);

	schema sch;
	sch.AddDevices(&dc1, &dc2, &dc3, &dc4, &dc5, &dc6, &dc7, 0);
	sch.AddDevices(&s1, &s2, &s3, &s4, &s5, &s6, &s7, 0);
	sch.AddDevices(&ss, &gnd, 0);
	sch.AddLines(&l1, &l2, &l3, &l4, &l5, &l6, &l7, 0);
	sch.AddLines(&l01, &l02, &l03, &l04, &l05, &l06, &l07, &l08, 0);
	sch.AddLines(&l11, &l22, &l33, &l44, &l55, &l66, &l77, 0);

	circuit cir(&sch);
	cir.Tran();
	do 
	{
		char key = getchar();
		switch (key)
		{
		case '1':
			cir.SwitchOver(&s1);
			break;
		case '2':
			cir.SwitchOver(&s2);
			break;
		case '3':
			cir.SwitchOver(&s3);
			break;
		case '4':
			cir.SwitchOver(&s4);
			break;
		case '5':
			cir.SwitchOver(&s5);
			break;
		case '6':
			cir.SwitchOver(&s6);
			break;
		case '7':
			cir.SwitchOver(&s7);
			break;
		case 'q':
			cir.Halt();
			break;
		}
		Sleep(100);
	} while (cir.IsRunning());
	getchar();
}
