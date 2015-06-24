#ifndef DEVICE_MIXED_H_
#define DEVICE_MIXED_H_

#include "generic.h"

class ngtimer : public ngdevice
{
public:
	ngtimer(string name, string subckt="LMC555_TIMER");

	string card();

#define TM_GND p1
#define TM_TRI p2
#define TM_OUT p3
#define TM_RST p4
#define TM_CON p5
#define TM_THR p6
#define TM_DIS p7
#define TM_VCC p8
};

#endif