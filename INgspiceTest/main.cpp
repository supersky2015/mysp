#include "stdafx.h"
#include <functional>
#include <iostream>
#include "test-ingspice.h"
#include "test-sim.h"
#include "test-schema.h"
#include "test-circuit.h"
#include "test-555.h"
#include "test-model-subckt.h"
#include "test-8-way-responder.h"
#include "test-sources.h"
#include "test-device-basic.h"
#include <include/subckt.h>
#include <common/common.h>

void test_path_hpp();
void test_string_hpp();

//uncomment one to test
int main (int argc, char **argv)
{
	//basic
	//test_basic_ngspst_pack();
	test_basic_ngspdt_pack();

	//sources
	//test_ac_and_indicator();
	//test_vcvs();
	//test_cccs();
	//test_vccs();
	//test_ccvs();
	//test_ac_current();
	//test_dc_current();

	//8 way responder
	//test_7seg();

	//555
	//test_555_monostable();
	//test_555_monostable_running_error();
	//test_555_astable();

	// circuit class
	//test_rc_charge_discharge();
	//test_circuit_rc_tran();
	//test_spdt();
	//test_switch_by_csw();
	//test_switch_by_csw_inter();
	//test_switch_by_resistor();
	//test_circuit_rc_charge();
	//test_flash_led();
	//test_restart();
	//test_circuit_led();
	//test_circuit_parallel();
	//test_losing_state();

	// common
	//test_string_hpp();
	//test_path_hpp();

	// model & subckt
	//test_model_subckt();

	// schema
	//test_schema_rc();
	//test_schema_multiline();
	test_schema_connection();

	// ngspice interface
	//return run_parallel();
	//return load_error();
	//return load_error2();
	//return run_error();
	//return test_rc();
	//return test_3p_effective_voltage();
	//return test_xspice();

	// ngspice examples
	//test_sim_agent();
	//test_sim_alter_agent();

	system("pause");
	return 0;
}