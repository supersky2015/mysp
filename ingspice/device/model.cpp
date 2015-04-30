#include "stdafx.h"
#include <fstream>
#include <include/model.h>
#include <common/common.h>

model_helper* model_helper::instance = NULL;

model_helper* model_helper::Instance()
{
	if (!instance)
		instance = new model_helper();

	return instance;
}

std::string model_helper::GetModel( string model )
{
	return models.end() != models.find(model) ? models[model] : "";
}

model_helper::model_helper()
{
	string ini = get_path_directory(__FILE__) + "models.ini";

	fstream ifs(ini, ios::in|ios::binary);
	string model;
	while (ifs.good()){
		string line;
		getline(ifs, line);
		string ln = get_strip(line, " \r\n");
		if (has_quote(ln, '[')){
			model = unquote(ln, ']');
		}else{
			if (!line.empty() && line[0] == '#')
				continue;
			if (model.empty())
				continue;
			models[model].append(line);
		}
	}
	ifs.close();
}
