#include "stdafx.h"
#include <include/basic.h>
#include <common/common.h>

const double ngswitch::on = 1e-10;
const double ngswitch::off = 1e20;

std::string ngresistor::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" %g", r);
}

std::string ngcapacitor::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" %g", this->c);
}

std::string nginductance::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" %g", l);
}

std::string ngswitch::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" %g", r);
}

std::string ngswitch::connect()
{
	r = on;
	return format_string("alter r%s=%g", name.c_str(), r);
}

std::string ngswitch::disconnect()
{
	r = off;
	return format_string("alter r%s=%g", name.c_str(), r);
}

std::string ngswitch::switchover()
{
	if (on == r)
		r = off;
	else if (off == r)
		r = on;

	return format_string("alter r%s=%g", name.c_str(), r);
}

ngspst::ngspst( string name, int st/* = ngspst::off*/)
	:ngdevice('X', name, 2)
	,status(st)
{
	ngdevice::subckt = "spst";
}

std::string ngspst::switchover()
{
	status = (on == status) ? off : on;
	return format_string("alter v.x%s.v1=%d", name.c_str(), on == status ? -2 : 0);
}

std::string ngspst::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" spst params:vstatus=%d", on == status ? -2 : 0);
}

string ngspst::connect()
{
	status = on;
	return format_string("alter v.x%s.v1=-2", name.c_str());
}

string ngspst::disconnect()
{
	status = off;
	return format_string("alter v.x%s.v1=0", name.c_str());
}

ngspdt::ngspdt( string name, int st /*= status_throw1*/ )
	:ngdevice('X', name, 3)
	,status(st)
{
	ngdevice::subckt = "spdt";
}

std::string ngspdt::switchover()
{
	status = (status_throw1 == status) ? status_throw2 : status_throw1;
	return format_string("alter v.x%s.v1=%d", name.c_str(), status_throw1 == status ? 0 : -2);
}

std::string ngspdt::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" spdt params:vstatus=%d", status_throw1 == status ? 0 : -2);
}

string ngspdt::TurnThrow1()
{
	status = status_throw1;
	return format_string("alter v.x%s.v1=0", name.c_str());
}

string ngspdt::TurnThrow2()
{
	status = status_throw2;
	return format_string("alter v.x%s.v1=-2", name.c_str());
}

ngspst_pack::ngspst_pack( string name, int pack_count, int st /*= ngspst::off*/ )
	:ngdevice(' ', name, 2*pack_count)
	,status(st)
{
	ngdevice::subckt = "spst";
	for (int i = 0; i < pack_count; i++)
	{
		string nm = format_string("spst_pack_%s_%d", name.c_str(), i);
		ngspst* spst = new ngspst(nm, st);
		spsts_.push_back(spst);
	}
}

ngspst_pack::~ngspst_pack()
{
	for (size_t i = 0; i < spsts_.size(); i++)
	{
		delete spsts_[i];
	}
}

string ngspst_pack::card()
{
	string c;
	for (size_t i = 0; i < spsts_.size(); i++)
	{
		format_append(c, "%s\n", spsts_[i]->card().c_str());
	}
	return c;
}

string& ngspst_pack::orders( int index )
{
	return spsts_[index/2]->orders(index%2);
}

ngcontact ngspst_pack::pin( int p )
{
	return spsts_[p/2]->pin(p%2);
}

string ngspst_pack::connect()
{
	string cmd;
	for (size_t i = 0; i < spsts_.size(); i++)
	{
		format_append(cmd, "%s\n", spsts_[i]->connect().c_str());
	}
	return cmd;
}

string ngspst_pack::disconnect()
{
	string cmd;
	for (size_t i = 0; i < spsts_.size(); i++)
	{
		format_append(cmd, "%s\n", spsts_[i]->disconnect().c_str());
	}
	return cmd;
}

string ngspst_pack::switchover()
{
	string cmd;
	for (size_t i = 0; i < spsts_.size(); i++)
	{
		format_append(cmd, "%s\n", spsts_[i]->switchover().c_str());
	}
	return cmd;
}

void ngspst_pack::SetAllowOpen( vector<long> ao )
{
	if (ao.size() != 2*spsts_.size())
		return;

	for (size_t i = 0; i < spsts_.size(); i++)
	{
		vector<long> allow_open;
		allow_open.assign(ao.begin() + 2*i, ao.begin() + 2*i + 2);
		spsts_[i]->SetAllowOpen(allow_open);
	}
}
