#ifndef TRANSISTOR_H
#define TRANSISTOR_H

#include "generic.h"

class ngbjt_npn : public ngdevice
{
public:
	ngbjt_npn(string name, string model = "generic-npn");

	string card();

#define coll p1
#define base p2
#define emit p3
};

#endif