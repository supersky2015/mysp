#include "stdafx.h"
#include <include/plot.h>

double plot::GetVecCurrValue( const string& name )
{
	for (size_t i = 0; i < names.size(); i++)
	{
		if (0 == _stricmp(name.c_str(), names[i].c_str()))
			return currentValues[i];
	}
	return 0;
}
