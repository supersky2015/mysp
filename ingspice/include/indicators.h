#ifndef DEVICE_INDICATORS_H_
#define DEVICE_INDICATORS_H_

#include "generic.h"

// seven segment led
class ngseven_seg : public ngdevice, public ngaction
{
public:
	//seven_seg_com_pos, common positive
	//seven_seg_com_neg, common negative
	ngseven_seg(string name, string subckt = "seven_seg_com_neg", double light_current = 5e-3);

	//get device card
	string card();

	//device action animation
	bool action(double time);

	inline int digital(){return digital_;}

	inline unsigned char code(){return code_;}

private:
	/*
	*	     --a1--
	*		|      |
	*		f6     b2
	*		|--g7--|
	*		e5     c3
	*		|      |
	*		 --d4--
	*		 
	*	  digital   code1(bin)	code1(hex)				code2(hex)
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
	int digital_;

	// current code indicated.
	unsigned char code_;

	// if current of a seg is bigger than lightCurrent, the seg lights up.
	double light_current_;

#define SS_A p1
#define SS_B p2
#define SS_C p3
#define SS_D p4
#define SS_E p5
#define SS_F p6
#define SS_G p7
#define SS_COM p8
};

class ngvoltmeter : public ngdevice, public ngaction
{
public:
	ngvoltmeter(string name, double ohm = 1e10)
		:ngdevice('R', name, 2)
		,ohm_(ohm)
		,voltage_(0.0)
	{
	}

	string card();

	bool action(double time);

	inline double voltage(){return voltage_;}

#define V_POS p1
#define V_NEG p2

private:
	// voltage measured
	double voltage_;

	// inner ohm
	double ohm_;
};

class ngammeter : public ngdevice, public ngaction
{
public:
	ngammeter(string name);

	string card();

	bool action(double time);

	inline double current(){return current_;}

#define A_POS p1
#define A_NEG p2

private:
	double current_;
};

// ac voltmeter that measures effective voltage
class ngac_voltmeter : public ngdevice, public ngaction
{
public:
	ngac_voltmeter(string name, double ohm = 1e10)
		:ngdevice('R', name, 2)
		,ohm_(ohm)
		,voltage_(0.0)
		,count_(0)
		,sum_square_(0.0)
	{
	}

	string card();

	bool action(double time);

	inline double voltage(){return voltage_;}

private:
	// effective voltage measured, root-mean-square(RMS)
	double voltage_;

	// inner ohm
	double ohm_;

	// count of steps
	unsigned int count_;

	// sum of square
	double sum_square_;
};

// ac ammeter that measures effective current
class ngac_ammeter : public ngdevice, public ngaction
{
public:
	ngac_ammeter(string name);

	string card();

	bool action(double time);

	inline double current(){return current_;}

private:
	// effective current measured. root-mean-square(RMS)
	double current_;

	// count of steps
	unsigned int count_;

	// sum of square
	double sum_square_;
};

#endif