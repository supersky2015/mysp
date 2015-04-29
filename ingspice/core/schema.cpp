#include "stdafx.h"
#include <set>
#include <algorithm>
using namespace std;

#include <include/schema.h>
#include <common/common.h>

schema::schema( string name )
{
	this->name = name;
}

bool schema::AddDevice( ngdevice* device )
{
	auto r = find(devices.begin(), devices.end(), device);
	if (r == devices.end()){
		devices.push_back(device);
		return true;
	}
	return false;
}

bool schema::RemoveDevice( ngdevice* device )
{
	auto r = find(devices.begin(), devices.end(), device);
	if (r != devices.end()){
		devices.erase(r);
		return true;
	}
	return false;
}

bool schema::RemoveLine( ngline* line )
{
	auto r = find(lines.begin(), lines.end(), line);
	if (r != lines.end()){
		lines.erase(r);
		return true;
	}
	return false;
}

bool schema::AddLine( ngline* line )
{
	auto r = find(lines.begin(), lines.end(), line);
	if (r == lines.end()){
		lines.push_back(line);
		return true;
	}
	return false;
}

bool schema::sort()
{
	//reset orders of contacts, both lines and devices
	for (size_t i = 0; i < devices.size(); i++){
		size_t pinCount = devices[i]->orders.size();
		devices[i]->orders.assign(pinCount, "-1");
	}
	for (size_t i = 0; i < lines.size(); i++){
		lines[i]->order = -1;
	}


	//group contacts with same potentials
	int order = 1;
	bool hasGround = false;
	for (size_t i = 0; i < lines.size(); i++){
		for (size_t j = i + 1; j < lines.size(); j++){
			if (lines[j]->order != -1)
				continue;

			if (isConnected(lines[i], lines[j])){
				if (isGround(lines[i]->c1) || isGround(lines[i]->c2) || isGround(lines[j]->c1) || isGround(lines[j]->c2)){
					lines[i]->order = lines[j]->order = 0;
					hasGround = true;
				}else
					lines[i]->order = lines[j]->order = order++;
			}
		}
	}

	for (size_t i = 0; i < lines.size(); i++){
		if (lines[i]->order == -1)
			lines[i]->order = order++;
	}

	if (!hasGround)
		return false;

	//map devices to line order
	for (size_t i = 0; i < devices.size(); i++){
		for (size_t j = 0; j < devices[i]->pins.size(); j++){
			for (size_t k = 0; k < lines.size(); k++){
				//if device pin(contact) connected to a line, set order as the line order
				if (isConnected(&(*devices[i])[j], lines[k])){
					devices[i]->orders[j] = FormatString(10, "%d", lines[k]->order);
					break;
				}
			}
		}
	}

	return true;
}

bool schema::isConnected( ngline* line1, ngline* line2 )
{
	return line1->c1 == line2->c1 || line1->c1 == line2->c2 || line1->c2 == line2->c1 || line1->c2 == line2->c2;
}

bool schema::isConnected( ngcontact* contact, ngline* line )
{
	return (contact->name == line->c1.name && contact->pin == line->c1.pin) || (contact->name == line->c2.name && contact->pin == line->c2.pin);
}

bool schema::isGround( const ngcontact& contact )
{
	ngdevice* dev = getDeviceByName(contact.name);
	if (!dev)
		return false;
	return NULL != dynamic_cast<ngground*>(dev);
}

ngdevice* schema::getDeviceByName( const string& name )
{
	size_t size = devices.size();
	for (size_t i = 0; i < size; i++){
		if (devices[i]->name == name)
			return devices[i];
	}
	return NULL;
}

//call this after sort
vector<string> schema::GetNetlist()
{
	sort();
	vector<string> netlist;
	netlist.push_back("title");
	for (size_t i = 0; i < devices.size(); i++){
		string line = devices[i]->netlist();
		netlist.push_back(line);
	}
	netlist.push_back(".end");
	return netlist;
}

std::string schema::getModels()
{
	set<string> models;
	for (size_t i = 0; i < devices.size(); i++){
		if (!devices[i]->model.empty()){
			models.insert(devices[i]->model);
		}
	}

	string def;
	

	return def;
}

std::string schema::getSubckts()
{
	set<string> subckts;
	for (size_t i = 0; i < devices.size(); i++){

	}
	string def;
	return def;
}
