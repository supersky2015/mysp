#include "StdAfx.h"
#include <algorithm>
#include <functional>
#include <memory>
#include <vector>
#include <common/common.h>
#include <include/circuit.h>
#include <include/device.h>
#include <include/schema.h>
#include <include/ngspice.h>

using namespace std;

circuit::circuit(schema* sch)
	:sch(sch)
{
}

circuit::~circuit(void)
{
}

bool circuit::Tran(const char* max /*= "1t"*/, const char* step /*= "10u"*/, const char* ic /*= " uic" */)
{
	//combine netlist and command
	vector<string> netlist = sch->GetNetlist();
	string tran = format_string(".tran %s %s", step, max);
	if (ic)
		tran.append(ic);
	updateNetlist(netlist, tran);

	//try to load netlist
	if (!ngspice::LoadNetlist(netlist))
		return false;

	//start ngspice simulation
	return ngspice::Run();
}

bool circuit::updateNetlist( vector<string>& netlist, string command )
{
	auto it = find(netlist.begin(), netlist.end(), ".end");
	if (it == netlist.end())
		return false;
	netlist.insert(it, command);
	return true;
}

void circuit::SimAction(double time)
{
	//update all devices contact voltage.
	for (size_t i = 0; sch && i < sch->devices.size(); i++)
	{
		ngdevice* dev = sch->devices[i];
		for (int j = 0; j < dev->port_count(); j++)
		{
			if (dev->orders(j) == "0")
				dev->potentials(j) = 0.0;
			else
			{
				string name = format_string("V(%s)", dev->orders(j).c_str());
				dev->potentials(j) = m_plot.GetVecCurrValue(name);
			}
		}
	}

	//update all branches current
	for (size_t i = 0; sch && i < sch->devices.size(); i++)
	{
		ngdevice* dev = sch->devices[i];
		for (size_t j = 0; j < dev->branches.size(); j++)
		{
			dev->currents[j] = m_plot.GetVecCurrValue(dev->branches[j].c_str());
		}
	}

	//do action if there is
	for (size_t i = 0; sch && i < sch->devices.size(); i++)
	{
		ngaction* act = dynamic_cast<ngaction*>(sch->devices[i]);
		if (act)
			act->action(time);
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
	if (!ngspice::IsRunning())
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
	bool ret = ngspice::Halt();
	ret &= ngspice::Do(cmd.c_str());
	ret &= ngspice::Resume();

	return ret;
}

bool circuit::Execute( const string& cmd )
{
	if (!ngspice::IsRunning())
		return false;

	bool ret = ngspice::Halt();
	// if device is composite device, that contains devices as member. such as ngspst_pack
	vector<string> cmds = split(cmd, "\n");
	for (size_t i = 0; i < cmds.size(); i++ )
		ret &= ngspice::Do(cmds[i].c_str());
	ret &= ngspice::Resume();
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

bool circuit::Wait(float time/* = -1*/)
{
	do
	{
		Sleep(100);
		string err = ngspice::ErrorMsgRuning();
		if (!err.empty())
		{
			PRINT(" <error msg=%s\n/>", err.c_str());
			Halt();
			return false;
		}
		if (time > 0 && ngspice::CurrentValue("time") >= time)
		{
			Halt();
			return false;
		}
	} while (IsRunning());
	return true;
}
