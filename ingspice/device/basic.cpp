#include "stdafx.h"
#include "include/basic.h"
#include <assert.h>
#include "common/common.h"

const double ngswitch::on = 1e-10;
const double ngswitch::off = 1e20;

std::string ngresistor::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" %g", r_);
}

std::string ngresistor::AlterResistance( double r )
{
	r_ = r;
	return format_string("alter r%s=%g", ngdevice::name.c_str(), r);
}

std::string ngcapacitor::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" %g", c_);
}

std::string ngcapacitor::AlterCapacitor( double c )
{
	c_ = c;
	return format_string("alter c%s=%g", ngdevice::name.c_str(), c);
}

std::string nginductance::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" %g", l_);
}

std::string nginductance::AlterInductance( double l )
{
	l_ = l;
	return format_string("alter l%s=%g", ngdevice::name.c_str(), l);
}

std::string ngswitch::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" %g", r_);
}

std::string ngswitch::connect()
{
	r_ = on;
	return format_string("alter r%s=%g", name.c_str(), r_);
}

std::string ngswitch::disconnect()
{
	r_ = off;
	return format_string("alter r%s=%g", name.c_str(), r_);
}

std::string ngswitch::switchover()
{
	if (on == r_)
		r_ = off;
	else if (off == r_)
		r_ = on;

	return format_string("alter r%s=%g", name.c_str(), r_);
}

ngspst::ngspst( string name, int st/* = ngspst::off*/)
	:ngdevice('X', name, 2)
	,status_(st)
{
	ngdevice::subckt = "spst";
}

std::string ngspst::switchover()
{
	status_ = (on == status_) ? off : on;
	return format_string("alter v.x%s.v1=%d", name.c_str(), on == status_ ? -2 : 0);
}

std::string ngspst::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" spst params:vstatus=%d", on == status_ ? -2 : 0);
}

string ngspst::connect()
{
	status_ = on;
	return format_string("alter v.x%s.v1=-2", name.c_str());
}

string ngspst::disconnect()
{
	status_ = off;
	return format_string("alter v.x%s.v1=0", name.c_str());
}

ngspdt::ngspdt( string name, int st /*= status_throw1*/ )
	:ngdevice('X', name, 3)
	,status_(st)
{
	ngdevice::subckt = "spdt";
}

std::string ngspdt::switchover()
{
	status_ = (status_throw1 == status_) ? status_throw2 : status_throw1;
	return format_string("alter v.x%s.v1=%d", name.c_str(), status_throw1 == status_ ? 0 : -2);
}

std::string ngspdt::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" spdt params:vstatus=%d", status_throw1 == status_ ? 0 : -2);
}

string ngspdt::TurnThrow1()
{
	status_ = status_throw1;
	return format_string("alter v.x%s.v1=0", name.c_str());
}

string ngspdt::TurnThrow2()
{
	status_ = status_throw2;
	return format_string("alter v.x%s.v1=-2", name.c_str());
}

ngspst_pack::ngspst_pack( string name, int pack_count, int st /*= ngspst::off*/ )
	:ngdevice('*', name, 2*pack_count)
	,pack_count_(pack_count)
	,status_(st)
{
	ngdevice::subckt = "spst";
	for (int i = 0; i < pack_count_; i++)
	{
		string nm = format_string("spst_pack_%s_%d", name.c_str(), i);
		ngspst* spst = new ngspst(nm, st);
		spsts_.push_back(spst);
	}
}

ngspst_pack::~ngspst_pack()
{
	for (int i = 0; i < pack_count_; i++)
	{
		delete spsts_[i];
	}
	spsts_.clear();
}

string ngspst_pack::card()
{
	string c;
	for (int i = 0; i < pack_count_; i++)
	{
		format_append(c, "%s\n", spsts_[i]->card().c_str());
	}
	return c;
}

string& ngspst_pack::orders( int index )
{
	assert(index >= 0 && index < pack_count_*2);
	return spsts_[index/2]->orders(index%2);
}

ngcontact ngspst_pack::pin( int p )
{
	assert(p >= 0 && p < pack_count_*2);
	return spsts_[p/2]->pin(p%2);
}

string ngspst_pack::connect(int index/* = -1*/)
{
	if (index >= 0 && index < pack_count_)
		return spsts_[index]->connect();

	string cmd;
	for (int i = 0; i < pack_count_; i++)
	{
		format_append(cmd, "%s\n", spsts_[i]->connect().c_str());
	}
	return cmd;
}

string ngspst_pack::disconnect(int index/* = -1*/)
{
	if (index >= 0 && index < pack_count_)
		return spsts_[index]->disconnect();

	string cmd;
	for (size_t i = 0; i < spsts_.size(); i++)
	{
		format_append(cmd, "%s\n", spsts_[i]->disconnect().c_str());
	}
	return cmd;
}

string ngspst_pack::switchover(int index/* = -1*/)
{
	if (index >= 0 && index < pack_count_)
		return spsts_[index]->switchover();

	string cmd;
	for (int i = 0; i < pack_count_; i++)
	{
		format_append(cmd, "%s\n", spsts_[i]->switchover().c_str());
	}
	return cmd;
}

void ngspst_pack::SetAllowOpen( vector<long> ao )
{
	if (ao.size() != pack_count_*2)
	{
		ao.clear();
		ao.assign(pack_count_*2, 0);
	}

	for (size_t i = 0; i < spsts_.size(); i++)
	{
		vector<long> allow_open;
		allow_open.assign(ao.begin() + 2*i, ao.begin() + 2*i + 2);
		spsts_[i]->SetAllowOpen(allow_open);
	}
}

ngspdt_pack::ngspdt_pack( string name, int pack_count, int state /*= status_throw1*/ )
	:ngdevice('*', name, 3*pack_count)
	,pack_count_(pack_count)
	,status_(state)
{
	ngdevice::subckt = "spdt";
	for (int i = 0; i < pack_count_; i++)
	{
		string nm = format_string("spdt_pack_%s_%d", name.c_str(), i);
		ngspdt* spdt = new ngspdt(nm, state);
		spdts_.push_back(spdt);
	}
}

ngspdt_pack::~ngspdt_pack()
{
	for (int i = 0; i < pack_count_; i++)
	{
		delete spdts_[i];
	}
	spdts_.clear();
}

string ngspdt_pack::card()
{
	string c;
	for (int i = 0; i < pack_count_; i++)
	{
		format_append(c, "%s\n", spdts_[i]->card().c_str());
	}
	return c;
}

string ngspdt_pack::switchover( int index /*= all*/ )
{
	if (index >= 0 && index < pack_count_)
		return spdts_[index]->switchover();

	string cmd;
	for (int i = 0; i < pack_count_; i++)
	{
		format_append(cmd, "%s\n", spdts_[i]->switchover().c_str());
	}
	return cmd;
}

string ngspdt_pack::TurnThrow1( int index /*= all*/ )
{
	if (index >= 0 && index < pack_count_)
		return spdts_[index]->TurnThrow1();

	string cmd;
	for (int i = 0; i < pack_count_; i++)
	{
		format_append(cmd, "%s\n", spdts_[i]->TurnThrow1().c_str());
	}
	return cmd;
}

string ngspdt_pack::TurnThrow2( int index /*= all*/ )
{
	if (index >= 0 && index < pack_count_)
		return spdts_[index]->TurnThrow2();

	string cmd;
	for (int i = 0; i < pack_count_; i++)
	{
		format_append(cmd, "%s\n", spdts_[i]->TurnThrow2().c_str());
	}
	return cmd;
}

string& ngspdt_pack::orders( int index )
{
	assert(index >= 0 && index < pack_count_*3);
	return spdts_[index/3]->orders(index%3);
}

ngcontact ngspdt_pack::pin( int p )
{
	assert(p >= 0 && p < pack_count_*3);
	return spdts_[p/3]->pin(p%3);
}

void ngspdt_pack::SetAllowOpen( vector<long> ao )
{
	if (ao.size() != pack_count_*3)
	{
		ao.clear();
		ao.assign(pack_count_*3, 0);
	}

	for (int i = 0; i < pack_count_; i++)
	{
		vector<long> opens;
		opens.assign(ao.begin() + 3*i, ao.begin() + 3*i + 3);
		spdts_[i]->SetAllowOpen(opens);
	}
}
