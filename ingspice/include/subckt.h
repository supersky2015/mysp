#ifndef SUBCKT_H
#define SUBCKT_H
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