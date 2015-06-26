#ifndef CORE_SCHEMA_H_
#define CORE_SCHEMA_H_

#include <vector>
using namespace std;

#include "device.h"

class schema
{
public:
	schema(string name = "schema");
	
	// add devices to this schema
	bool AddDevice(ngdevice* device);
	void AddDevices(ngdevice* device, ...);
	void AddDevices(vector<ngdevice*> ds);
	
	// remove devices from schema
	bool RemoveDevice(ngdevice* device);
	void RemoveDevices(ngdevice* device, ...);
	void RemoveDevices(vector<ngdevice*> ds);
	void RemoveAllDevices();
	
	// add lines to schema
	bool AddLine(ngline* line);
	void AddLines(ngline* line, ...);
	void AddLines(vector<ngline*> ls);
	
	//remove lines from schema
	bool RemoveLine(ngline* line);
	void RemoveLines(ngline* line, ...);
	void RemoveLines(vector<ngline*> ls);
	void RemoveAllLines();

	//
	inline void set_print_sort(bool print_sort){print_sort_ = print_sort;};

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
	void debugLines();

	// sort schema to get all potential nodes order
	bool sort();

	// get all models card used by this schema
	string getModels();

	// get all subckts card used by this schema
	string getSubckts();

private:
	// if print sort procedure debug info
	bool print_sort_;
};

#endif