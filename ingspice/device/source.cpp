#include "stdafx.h"
#include "include/source.h"
#include "common/common.h"


string ngground::card()
{
	return "";	
}

std::string ngdc::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" dc %g", v_);
}

std::string ngdc::AlterVoltage( double v )
{
	v_ = v;
	return format_string("alter v%s=%g", ngdevice::name.c_str(), v);
}

std::string ngac::card()
{
	// for some reason unknown yet, "dc" is required.
	// otherwise, error with "no dc value, transient time 0 value used" prompted
	// see test_ac_and_indicator() in test-sources.cpp
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" dc 0 sin(%g %g %g %g 0 %g)", o_, a_, f_, d_, p_);
}

std::string ngpluse::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" pulse(%g %g %g %g %g %g %g)", v1_, v2_, td_, tr_, tf_, pw_, per_);
}

string ngdc_current::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" dc %g", i_);
}

std::string ngdc_current::AlterCurrent( double i )
{
	i_ = i;
	return format_string("alter i%s=%g", ngdevice::name.c_str(), i);
}

string ngac_current::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" dc 0 sin(%g %g %g %g 0 %g)", o_, a_, f_, d_, p_);
}

string ngvcvs::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" %g", value_);
}

string ngcccs::card()
{
	string c = ngdevice::subckt_card();
	return c.empty() ? "" : c + format_string(" params: value=%g", value_);
}

string ngvccs::card()
{
	string c = ngdevice::card();
	return c.empty() ? "" : c + format_string(" %g", value_);
}

string ngccvs::card()
{
	string c = ngdevice::subckt_card();
	return c.empty() ? "" : c + format_string(" params: value=%g", value_);
}
