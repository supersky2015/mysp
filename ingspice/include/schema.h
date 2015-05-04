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

	bool AddDevice(ngdevice* device);
	void AddDevices(ngdevice* device, ...);
	bool RemoveDevice(ngdevice* device);
	void RemoveDevices(ngdevice* device, ...);
	bool AddLine(ngline* line);
	void AddLines(ngline* line, ...);
	bool RemoveLine(ngline* line);
	void RemoveLines(ngline* line, ...);

	vector<string> GetNetlist();

	vector<ngdevice*> devices;
	vector<ngline*> lines;

	string name;

private:
	bool isConnected(ngcontact* contact, ngline* line );
	bool isConnected(ngline* line1, ngline* line2);
	bool isGround(const ngcontact& contact);
	ngdevice* getDeviceByName(const string& name);
	bool sort(); 

	string getModels();
	string getSubckts();
};

#endif