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

ngdevice::ngdevice(string name, int portCount )
{
	this->name = name;

	orders.assign(portCount, "-1");
	pins.assign(portCount, "");
	for (int i = 0; i < portCount; i++){
		pins[i] = FormatString(10, "%d", i);
	}

	model.clear();
	subckt.clear();
}

std::string ngdevice::netlist()
{
	return "";
}

ngcontact ngdevice::operator[]( int pin )
{
	assert(pin >= 0 && pin < (int)pins.size());
	return ngcontact(name, pins[pin]);
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

//ngcontact::ngcontact( const ngcontact& contact )
//{
//	this->name = contact.name;
//	this->pin = contact.pin;
//}

//ngcontact& ngcontact::operator=( const ngcontact& rhs )
//{
//	this->name = rhs.name;
//	this->pin = rhs.pin;
//	return *this;
//}
