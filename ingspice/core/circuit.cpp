#include "StdAfx.h"
#include <algorithm>
#include <functional>
#include <memory>
#include <include/circuit.h>
#include <include/device.h>
#include <include/schema.h>
#include <include/ngspice.h>
#include <common/common.h>

using namespace std;

circuit::circuit(schema* sch)
	:sch(sch)
{
	ng = new ngspice(this, &circuit::schemaAction);
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
	return ng->Run();
}

bool circuit::Stop()
{
	return ng->Halt();
}

bool circuit::Resume()
{
	return ng->Resume();
}

bool circuit::Restart()
{
	return ng->Halt() && ng->Run();
}

bool circuit::Do( string cmd )
{
	return ng->Do(cmd.c_str());
}

bool circuit::Tran(const char* max /*= "1t"*/, const char* step /*= "10u"*/, const char* ic /*= " uic" */)
{
	//combine netlist and command
	vector<string> netlist = sch->GetNetlist();
	string tran = format_string(".tran %s %s", step, max);
	if (ic)
		tran.append(ic);
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
	schema* sch = this->Schema();
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

bool circuit::TurnOn( ngdevice* sw )
{
	return turnSwitch(sw, circuit::on);
}

bool circuit::TurnOff( ngdevice* sw )
{
	return turnSwitch(sw, circuit::off);
}

bool circuit::SwitchOver( ngdevice* sw )
{
	return turnSwitch(sw, circuit::switchover);
}

bool circuit::turnSwitch( ngdevice* sw, int status /*=switchover*/ )
{
	if (!ng->IsRunning())
		return false;

	string cmd;
	if (ngswitch* s = dynamic_cast<ngswitch*>(sw))
	{
		switch (status)
		{
		case on:
			cmd = s->connect();
			break;
		case off:
			cmd = s->disconnect();
			break;
		default:
			cmd = s->switchover();
			break;
		}
	}
	else if (ngspst* s = dynamic_cast<ngspst*>(sw))
	{
		cmd = s->switchover();
	}
	else if (ngspdt* s = dynamic_cast<ngspdt*>(sw))
	{
		cmd = s->switchover();
	}
	else
		return false;

	// halt simulation to alter resistor
	bool ret = ng->Halt();
	ret &= ng->Do(cmd.c_str());
	ret &= ng->Resume();

	return ret;
}

bool circuit::Plot( string vec )
{
	DWORD tid = 0;
	vectoplot = std::make_shared<string>(vec);
	::CreateThread(NULL, 0, procPlot, this, 0, &tid);
	return true;
}

DWORD WINAPI circuit::procPlot(LPVOID param )
{
	circuit* cir = (circuit*)param;
	string cmd = format_string("plot %s", cir->vectoplot->data());
	cir->Do(cmd.c_str());
	// tofix: msg loop here.
	getchar();
	return 0;
}

double circuit::CurrentValue( string name )
{
	plot& p = ng->GetPlot();
	return p.GetVecCurrValue(name);
}
