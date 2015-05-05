#include "stdafx.h"

#include "test-ingspice.h"
#include "test-sim.h"
#include "test-schema.h"
#include "test-circuit.h"
#include "test-model-subckt.h"

#include <include/subckt.h>

void test_path_hpp();
void test_string_hpp();

int main (int argc, char **argv)
{
	//test_circuit1();
	//test_circuit_led();
	test_circuit_parallel();
	//test_string_hpp();
	//test_path_hpp();
	//test_model_subckt();
	//test_schema_rc();
	//test_schema_multiline();
	//return run_parallel();
	//return load_error();
	//return load_error2();
	//return run_error();
	//return test_rc();
	//return test_3p_effective_voltage();
	//return test_xspice();
	return 0;
}