#include "stdafx.h"
#include <assert.h>
#include <include/generic.h>
#include <common/common.h>


ngline::ngline( string name1, string pin1, string name2, string pin2 )
	:c1(name1, pin1)
	,c2(name2, pin2)
	,order(-1)
{
}

ngline::ngline( const ngcontact& c1, const ngcontact& c2 )
	:c1(c1)
	,c2(c2)
	,order(-1)
{
}

ngdevice::ngdevice(char type, string name, int portCount, int branchCount/* = 0 */)
	:type(type)
	,name(name)
{
	pins.assign(portCount, "");
	for (int i = 0; i < portCount; i++){
		pins[i] = FormatString(10, "%d", i);
	}

	orders.assign(portCount, "-1");
	potentials.assign(portCount, 0.0);
	
	branches.resize(branchCount);
	currents.assign(branchCount, 0.0);

	model.clear();
	subckt.clear();
}

std::string ngdevice::card()
{
	return "";
}

std::string ngdevice::subckt_card()
{
	string crd = format_string("%c%s", type, name.c_str());
	for (size_t i = 0; i < orders.size(); i++)
	{
		//set a unique name if not connected(open)
		if ("-1" == orders[i])
			format_append(crd, " %s_OPEN_%s", name.c_str(), pins[i].c_str());
		else
			format_append(crd, " %s", orders[i].c_str());
	}
	format_append(crd, " %s", subckt.c_str());
	return crd;
}

std::string ngdevice::model_card()
{
	string crd = format_string("%c%s", type, name.c_str());
	for (size_t i = 0; i < orders.size(); i++)
	{
		//set a unique name if not connected(open)
		if ("-1" == orders[i])
			format_append(crd, " %s_OPEN_%s", name.c_str(), pins[i].c_str());
		else
			format_append(crd, " %s", orders[i].c_str());
	}
	format_append(crd, " %s", model.c_str());
	return crd;
}

ngcontact ngdevice::operator[]( int p )
{
	assert(p >= 0 && p < (int)pins.size());
	return ngcontact(name, pins[p]);
}

ngcontact ngdevice::pin(int p)
{
	return (*this)[p];
}

std::string ngdevice::vec( const ngcontact& contact )
{
	for (size_t i = 0; i < pins.size(); i++){
		if (pins[i] == contact.pin)
			return format_string("V(%s)", orders[i].c_str());
	}
	return "";
}

std::string ngdevice::vec( size_t n )
{
	if (n < 0 || n >= orders.size())
		return "";
	return format_string("V(%s)", orders[n].c_str());
}

std::string ngdevice::branch()
{
	return format_string("%c%s#branch", type, name.c_str());
}

std::string ngdevice::branch( string subType, string subName )
{
	return format_string("%s.%c%s.%s#branch", subType.c_str(), 'X', name.c_str(), subName.c_str());
}

bool ngcontact::operator==( const ngcontact& rhs )
{
	return (this->name == rhs.name && this->pin == rhs.pin);
}

ngcontact::ngcontact( string name, string pin )
{
	this->name = name;
	this->pin = pin;
}
