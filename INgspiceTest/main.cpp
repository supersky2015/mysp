#include "stdafx.h"

#include "test-ingspice.h"
#include "test-sim.h"
#include "test-schema.h"
#include "test-model-subckt.h"

#include <include/subckt.h>

int main (int argc, char **argv)
{
	test_model_subckt();
	//test_schema_rc();
	//return run_parallel();
	//return load_error();
	//return load_error2();
	//return run_error();
	//return test_rc();
	//return test_3p_effective_voltage();
	//return test_xspice();
	return 0;
}