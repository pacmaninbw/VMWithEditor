// State_Machine_Unit_Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "common_unit_test_logic.h"
#include "syntax_state_machine.h"
#include "internal_sytax_state_tests.h"
#include <stdio.h>
#include <stdlib.h>

static bool positive_path_state_machine_unit_tests(void)
{
	bool test_passed = true;

	test_passed = internal_tests_on_all_state_transitions(0);

	return test_passed;
}

static bool negative_path_state_machine_unit_tests(void)
{
	bool all_negative_unit_tests_passed = true;

	return all_negative_unit_tests_passed;
}

bool run_all_snstax_state_machine_unit_tests(void)
{
	bool all_unit_tests_passed = true;

	all_unit_tests_passed = positive_path_state_machine_unit_tests();

	return all_unit_tests_passed;
}

#ifndef ALL_UNIT_TESTING
int main()
{
	error_out_file = stderr;
	int passed = EXIT_SUCCESS;

	if (!init_vm_error_reporting(NULL) || !init_unit_tests("syntax_state_machine_unit_test_log.txt"))
	{
		return EXIT_FAILURE;
	}

	if (!run_all_snstax_state_machine_unit_tests())
	{
		passed = EXIT_FAILURE;
	}

	close_unit_tests();
	disengage_error_reporting();

	return passed;
}
#endif


