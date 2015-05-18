#ifndef SCHEMA_H
#define SCHEMA_H

#include <vector>
#include <map>
using namespace std;

#include <include/device.h>

class schema
{
public:
	schema(string name = "schema");
	
	// add devices to this schema
	bool AddDevice(ngdevice* device);
	void AddDevices(ngdevice* device, ...);
	
	// remove devices from schema
	bool RemoveDevice(ngdevice* device);
	void RemoveDevices(ngdevice* device, ...);
	
	// add lines to schema
	bool AddLine(ngline* line);
	void AddLines(ngline* line, ...);
	
	//remove lines from schema
	bool RemoveLine(ngline* line);
	void RemoveLines(ngline* line, ...);

	// generate netlist of this schema, for ngspice simulation
	vector<string> GetNetlist();

public:
	// all devices included in this schema
	vector<ngdevice*> devices;

	// all lines included in this schema
	vector<ngline*> lines;

	// schema name.
	string name;

private:
	// is contact connected to line
	bool isConnected(ngcontact* contact, ngline* line );

	// is line1 connected to line2
	bool isConnected(ngline* line1, ngline* line2);

	// is contact connected to ground
	bool isGround(const ngcontact& contact);

	// make sure name is unique.
	ngdevice* getDeviceByName(const string& name);

	// debug lines
	string debugLines();

	// sort schema to get all potential nodes order
	bool sort();

	// get all models card used by this schema
	string getModels();

	// get all subckts card used by this schema
	string getSubckts();

};

#endif