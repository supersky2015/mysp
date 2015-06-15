#include "stdafx.h"
#include <include/source.h>
#include <common/common.h>


string ngground::card()
{
	return "";	
}

std::string ngdc::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" dc %g", v);
}

std::string ngac::card()
{
	// for some reason unknown yet, "dc" is required.
	// otherwise, error with "no dc value, transient time 0 value used" prompted
	// see test_ac_and_indicator() in test-sources.cpp
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" dc 0 sin(%g %g %g %g 0 %g)", o, a, f, d, p);
}

std::string ngpluse::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" pulse(%g %g %g %g %g %g %g)", v1, v2, td, tr, tf, pw, per);
}

string ngdc_current::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" dc %g", i);
}

string ngac_current::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" dc 0 sin(%g %g %g %g 0 %g)", o, a, f, d, p);
}

string ngvcvs::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" %g", value);
}

string ngcccs::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" params: value=%g", value);
}

string ngvccs::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" %g", value);
}

string ngccvs::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" params: value=%g", value);
}
