#include "StdAfx.h"
#include <algorithm>
#include <include/circuit.h>
#include <include/device.h>
#include <include/schema.h>
#include <include/ngspice.h>
#include <common/common.h>

using namespace std;

circuit* circuit::s_this = NULL;

circuit::circuit(schema* sch)
	:sch(sch)
	,ng(new ngspice(schemaAction))
{
	s_this = this;
}

circuit::~circuit(void)
{
}

bool circuit::IsRunning() const
{
	return ng->IsRunning();
}

bool circuit::Run()
{
	return true;
}

bool circuit::Stop()
{
	return true;
}

bool circuit::Resume()
{
	return true;
}

bool circuit::Restart()
{
	return true;
}

bool circuit::Tran(const char* max /*= "1t"*/, const char* step /*= "10u"*/ )
{
	//combine netlist and command
	vector<string> netlist = sch->GetNetlist();
	string tran = format_string(".tran %s %s uic", step, max);
	updateNetlist(netlist, tran);

	//start ngspice simulation
	ng->LoadNetlist(netlist);
	return ng->Run();
}

bool circuit::updateNetlist( vector<string>& netlist, string command )
{
	auto it = find(netlist.begin(), netlist.end(), ".end");
	if (it == netlist.end())
		return false;
	netlist.insert(it, command);
	return true;
}

void circuit::schemaAction( ngspice* ng )
{
	schema* sch = s_this->Schema();
	plot& p = ng->GetPlot();

	//update all devices contact voltage.
	for (size_t i = 0; i < sch->devices.size(); i++)
	{
		ngdevice* dev = sch->devices[i];
		for (size_t j = 0; j < dev->orders.size(); j++)
		{
			if (dev->orders[j] == "0")
				dev->potentials[j] = 0.0;
			else
			{
				string name = format_string("V(%s)", dev->orders[j].c_str());
				dev->potentials[j] = p.GetVecCurrValue(name);
			}
		}
	}

	//update all branches current
	for (size_t i = 0; i < sch->devices.size(); i++)
	{
		ngdevice* dev = sch->devices[i];
		for (size_t j = 0; j < dev->branches.size(); j++)
		{
			dev->currents[j] = p.GetVecCurrValue(dev->branches[j].c_str());
		}
	}

	//do action if there is
	for (size_t i = 0; i < sch->devices.size(); i++)
	{
		ngaction* act = dynamic_cast<ngaction*>(sch->devices[i]);
		if (act)
			act->action();
	}

}
