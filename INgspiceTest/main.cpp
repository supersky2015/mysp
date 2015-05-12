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
	// circuit class
	test_circuit_rc_tran();
	//test_spdt();
	//test_switch_via_subckt();
	//test_switch_via_subckt_inter();
	//test_switch_led();
	//test_circuit_rc();
	//test_restart();
	//test_circuit_led();
	//test_circuit_parallel();

	// common
	//test_string_hpp();
	//test_path_hpp();

	// model & subckt
	//test_model_subckt();

	// schema
	//test_schema_rc();
	//test_schema_multiline();

	// ngspice interface
	//return run_parallel();
	//return load_error();
	//return load_error2();
	//return run_error();
	//return test_rc();
	//return test_3p_effective_voltage();
	//return test_xspice();

	// ngspice examples
	//test1::test_sim();
	//test_sim_alter();

	return 0;
}