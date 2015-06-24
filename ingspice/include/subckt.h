#ifndef DEVICE_SUBCKT_H_
#define DEVICE_SUBCKT_H_

#include <map>
using namespace std;

class subckt_helper{
public:
	static subckt_helper* Instance();
	string GetSubckt(string subckt);

private:
	subckt_helper();
	map<string, string> subckts;
	static subckt_helper* instance;
};


#endif