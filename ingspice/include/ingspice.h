#ifndef I_NGSPICE_H
#define I_NGSPICE_H
#include <string>
#include <vector>
using namespace std;

template<int N>
class port
{
public:
	port(){
		pins.assign(N, "");
	}

	string& operator[](int n){
		//assert(n >= 0 && n < N);
		return pins[n];
	}

	void reset(){
		for (int i = 0; i < N; i++){
			pins[i].clear();
		}
	}

private:
	vector<string> pins;
};

class ngaction 
{
public:
	virtual void callback();
};

class ngdevice
{
public:
	string name;
	virtual string netlist();
}; 


#endif