#ifndef DEVICE_INDICATORS_H
#define DEVICE_INDICATORS_H

#include "generic.h"

// seven segment led
class ngseven_seg : public ngdevice, public ngaction
{
public:
	//seven_seg_com_pos, common positive
	//seven_seg_com_neg, common negative
	ngseven_seg(string name, string subckt = "seven_seg_com_neg", double lightCurrent = 5e-3);

	//get device card
	string card();

	//device action animation
	void action(double time);

	/*
	*	     --a1--
	*		|      |
	*		f6     b2
	*		|--g7--|
	*		e5     c3
	*		|      |
	*		 --d4--
	*		 
	*	  digital   code(bin)	code(hex)
	*		0		011 1111	0x3f
	*		1		000 0110	0x06
	*		2		101 1011	0x5b
	*		3		100 1111	0x4f
	*		4		110	0110	0x66
	*		5		110 1101	0x6d
	*		6		111 1101	0x7d	or	111 1100	0x7c
	*		7		000	0111	0x07
	*		8		111 1111	0x7f
	*		9		110 1111	0x6f	or	110 0111	0x67
	*/

	// current digital indicated. 0 ~ 9.
	// if not match any from 0~9, digital is -1
	int digital;

	// current code indicated.
	unsigned char code;

	// if current of a seg is bigger than lightCurrent, the seg lights up.
	double lightCurrent;

#define SS_A p1
#define SS_B p2
#define SS_C p3
#define SS_D p4
#define SS_E p5
#define SS_F p6
#define SS_G p7
#define SS_COM p8
};


#endif