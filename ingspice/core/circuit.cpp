#include "StdAfx.h"
#include <include/circuit.h>


circuit::circuit(void)
{
}


circuit::~circuit(void)
{
}

bool circuit::IsRunning()
{
	return m_ngspice.IsRunning();
}

bool circuit::Run()
{
	if (IsRunning())
		return true;

	return m_ngspice.Run();
}

bool circuit::Stop()
{
	return m_ngspice.Halt();
}

bool circuit::Resume()
{
	return m_ngspice.Resume();
}

bool circuit::Restart()
{
	return m_ngspice.Halt() && m_ngspice.Run(); //to verify
}

