#include "stdafx.h"
#include <fstream>
#include <include/subckt.h>
#include <common/common.h>

subckt_helper* subckt_helper::instance = NULL;

subckt_helper::subckt_helper()
{
	string ini = GetPathDirectory(__FILE__) + "subckt.ini";

	string subckt;
	ifstream ifs(ini, ios::in|ios::binary);
	while (ifs.good()){
		string line;
		getline(ifs, line);
		string ln = get_strip(line, " \r\n");
		if (has_quote(ln, '[')){
			subckt = unquote(ln, '[');
		}else{
			if (!line.empty() && line[0] == '#') //# is custom comment tag
				continue;
			if (subckt.empty())
				continue;
			subckts[subckt].append(line);
		}
	}
}

std::string subckt_helper::GetSubckt( string subckt )
{
	return subckts.end() != subckts.find(subckt) ? subckts[subckt] : "";
}

subckt_helper* subckt_helper::Instance()
{
	if (!instance)
		instance = new subckt_helper();

	return instance;
}
