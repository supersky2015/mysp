#include "stdafx.h"
#include <set>
#include <vector>
#include <algorithm>
#include <fstream>
#include "common/common.h"
#include "include/model.h"
#include "include/subckt.h"
#include "include/schema.h" //tofix: before common.h can't build

using namespace std;

schema::schema( string name )
	:print_sort_(false)
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

void schema::AddDevices( ngdevice* device, ... )
{
	va_list ap;
	va_start(ap, device);
	ngdevice* dev = device;
	while (dev)
	{
		if (devices.end() == find(devices.begin(), devices.end(), dev))
			devices.push_back(dev);
		dev = va_arg(ap, ngdevice*);
	}
	va_end(ap);
}

void schema::AddDevices( vector<ngdevice*> ds )
{
	for (auto it = ds.begin(); it != ds.end(); it++)
	{
		if (devices.end() == find(devices.begin(), devices.end(), *it))
			devices.push_back(*it);
	}
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

void schema::RemoveDevices( ngdevice* device, ... )
{
	va_list ap;
	va_start(ap, device);
	ngdevice* dev = device;
	while (dev)
	{
		auto it = find(devices.begin(), devices.end(), device);
		if (it != devices.end())
			devices.erase(it);
		dev = va_arg(ap, ngdevice*);
	}
	va_end(ap);
}

void schema::RemoveDevices( vector<ngdevice*> ds )
{
	for (auto it = ds.begin(); it != ds.end(); it++)
	{
		auto p = find(devices.begin(), devices.end(), *it);
		if (p != devices.end())
			devices.erase(p);
	}
}

void schema::RemoveAllDevices()
{
	devices.clear();
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

void schema::RemoveLines( ngline* line, ... )
{
	va_list ap;
	va_start(ap, line);
	ngline* l = line;
	while (l)
	{
		auto it = find(lines.begin(), lines.end(), l);
		if (it != lines.end())
			lines.erase(it);
		l = va_arg(ap, ngline*);
	}
	va_end(ap);
}

void schema::RemoveLines( vector<ngline*> ls )
{
	for (auto it = ls.begin(); it != ls.end(); it++)
	{
		auto p = find(lines.begin(), lines.end(), *it);
		if (p != lines.end())
			lines.erase(p);
	}
}

void schema::RemoveAllLines()
{
	lines.clear();
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

void schema::AddLines( ngline* line, ... )
{
	va_list ap;
	va_start(ap, line);
	ngline* l = line;
	while (l){
		if (lines.end() == find(lines.begin(), lines.end(), l))
			lines.push_back(l);
		l = va_arg(ap, ngline*);
	}
	va_end(ap);
}

void schema::AddLines( vector<ngline*> ls )
{
	for (auto it = ls.begin(); it != ls.end(); it++)
	{
		if (lines.end() == find(lines.begin(), lines.end(), *it))
			lines.push_back(*it);
	}
}

#if 1
#define SORT_PRINT output_debug_message
#else
#define SORT_PRINT
#endif

void schema::debugLines()
{
	SORT_PRINT("-----------------\n");
	for (size_t i = 0; i < lines.size(); i++){
		SORT_PRINT("%s:%s - %s:%s -- %d\n", lines[i]->c1.name.c_str(), lines[i]->c1.pin.c_str(), lines[i]->c2.name.c_str(), lines[i]->c2.pin.c_str(), lines[i]->order);
	}
	SORT_PRINT("-----------------\n");
}

bool schema::sort()
{
	//reset orders of contacts, both lines and devices
	SORT_PRINT("1.reset orders of contacts, both lines and devices" );
	for (size_t i = 0; i < devices.size(); i++){
		for (int j = 0; j < devices[i]->port_count(); j++)
			devices[i]->orders(j) = "-1";
	}
	for (size_t i = 0; i < lines.size(); i++){
		lines[i]->order = -1;
	}

	//group contacts with same potentials
	SORT_PRINT("2.group contacts with same potentials" );
	int order = 1;
	bool hasGround = false;
	for (size_t i = 0; i < lines.size(); i++){
		// compare order , and set order
		for (size_t j = i + 1; j < lines.size(); j++){
			if (isConnected(lines[i], lines[j])){
				if (lines[i]->order == -1 && lines[j]->order == -1)
					lines[i]->order = lines[j]->order = order++;
				else if (lines[i]->order != -1 && lines[j]->order == -1)
					lines[j]->order = lines[i]->order;
				else if (lines[i]->order == -1 && lines[j]->order != -1)
					lines[i]->order = lines[j]->order;
			}
		}
		// if lines[i] connect to no line, set order
		// should set order after compare, not before compare
		if (lines[i]->order == -1)
			lines[i]->order = order++;
		if (print_sort_)
			debugLines();
	}
	
	//find ground, may be more than 1 ground
	SORT_PRINT("3.find ground, may be more than 1 ground" );
	vector<int> gndOrders;
	string grounds;
	for (size_t i = 0; i < lines.size(); i++){
		if (isGround(lines[i]->c1) || isGround(lines[i]->c2))
		{
			gndOrders.push_back(lines[i]->order);
			format_append(grounds, "%d ", lines[i]->order);
		}
	}
	SORT_PRINT(" grounds = %s", gndOrders.empty() ? "no ground" : grounds.c_str());

	//no ground
	if (gndOrders.empty())
		return false;

	//set line (which order is equal to gndOrder) order to 0, and set line (which order is greater than gndOrder) order minus 1;
	SORT_PRINT("4.set line (which order is equal to gndOrder) order to 0" );
	for (size_t i = 0; i < lines.size(); i++){
		if (gndOrders.end() != find(gndOrders.begin(), gndOrders.end(), lines[i]->order))
			lines[i]->order = 0;
	}

	//map devices to line order
	SORT_PRINT("5.map devices to line order" );
	for (size_t i = 0; i < devices.size(); i++){
		string ods;
		for (size_t j = 0; j < devices[i]->pins.size(); j++){
			for (size_t k = 0; k < lines.size(); k++){
				//if device pin(contact) connected to a line, set order as the line order
				if (isConnected(&(*devices[i]).pin(j), lines[k])){
					//output_debug_message("match");
					devices[i]->orders(j) = FormatString(10, "%d", lines[k]->order);
					break;
				}
			}
			format_append(ods, "%s ", devices[i]->orders(j).c_str());
		}
		if (print_sort_)
			SORT_PRINT(" name=%s orders=%s", devices[i]->name.c_str(), ods.c_str());
	}

	return true;
}

bool schema::isConnected( ngline* line1, ngline* line2 )
{
	return line1->c1 == line2->c1 || line1->c1 == line2->c2 || line1->c2 == line2->c1 || line1->c2 == line2->c2;
}

bool schema::isConnected( ngcontact* contact, ngline* line )
{
	//output_debug_message("%s:%s \t~\t [%s:%s - %s:%s  #%d]", contact->name.c_str(), contact->pin.c_str(), 
	//	line->c1.name.c_str(), line->c1.pin.c_str(), line->c2.name.c_str(), line->c2.pin.c_str(), line->order);
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
		string card = devices[i]->card();
		// if device is composite device, that contains devices as member. such as ngspst_pack
		vector<string> cards = split(card, "\n");
		netlist.insert(netlist.end(), cards.begin(), cards.end());
	}

	string models = getModels();
	vector<string> modelLines = split(models, "\r");
	netlist.insert(netlist.end(), modelLines.begin(), modelLines.end());

	string subckts = getSubckts();
	vector<string> subcktLines = split(subckts, "\r");
	netlist.insert(netlist.end(), subcktLines.begin(), subcktLines.end());

	netlist.push_back(".end");

	// save as debug info
	ofstream ofs("listing.cir");
	for (size_t i = 0; i < netlist.size(); i++)
	{
		ofs.write(netlist[i].c_str(), netlist[i].size());
		ofs.write("\n", 1);
	}
	ofs.close();

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
	for (auto it = models.begin(); it != models.end(); it++){
		def += model_helper::Instance()->GetModel(*it);
	}
	return def;
}

std::string schema::getSubckts()
{
	set<string> subckts;
	for (size_t i = 0; i < devices.size(); i++){
		if (!devices[i]->subckt.empty())
			subckts.insert(devices[i]->subckt);
	}
	
	string def;
	for (auto it = subckts.begin(); it != subckts.end(); it++){
		def += subckt_helper::Instance()->GetSubckt(*it);
	}
	return def;
}
