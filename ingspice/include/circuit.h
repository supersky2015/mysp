#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <vector>
using namespace std;

#include "device.h"
#include "ngspice.h"

class circuit
{
public:
	circuit(void);
	~circuit(void);

	bool IsRunning();

	bool Run();
	bool Stop();
	bool Resume();
	bool Restart();

private:

	
	ngspice m_ngspice;
};

#endif
