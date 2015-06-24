#ifndef DEVICE_MODEL_H_
#define DEVICE_MODEL_H_

#include <map>
using namespace std;

class model_helper{
public:
	static model_helper* Instance();

	string GetModel(string model);

private:
	model_helper();

	map<string, string> models;

	static model_helper* instance;
};

#endif