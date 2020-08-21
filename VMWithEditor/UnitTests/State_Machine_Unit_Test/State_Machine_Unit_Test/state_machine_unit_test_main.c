// state_machine_unit_test.c : This file contains the 'main' function. Program execution begins and ends there.
//
#include "common_unit_test_logic.h"
#include "lexical_analyzer.h"
#include "internal_sytax_state_tests.h"
#include <stdio.h>
#include <stdlib.h>

bool run_all_syntax_state_machine_unit_tests(unsigned test_step)
{
	bool all_unit_tests_passed = true;
	char buffer[BUFSIZ];

	sprintf(buffer, "Unit Test %zd: Starting Lexical Analizer Unit Tests \n\n", test_step);
	log_generic_message(buffer);

	all_unit_tests_passed = internal_tests_on_all_state_transitions(test_step);
	all_unit_tests_passed = true; // TODO remove after debug

	if (all_unit_tests_passed)
	{
		// test the public interface for the lexical analyzer
		all_unit_tests_passed = 
			unit_test_get_state_transition_collect_parser_error_data(test_step);
	}

	sprintf(buffer, "Unit Test %zd: run_all_syntax_state_machine_unit_tests(unsigned "
		"test_step) : %s\n\n", test_step, all_unit_tests_passed ? "Passed" : "Failed");
	log_generic_message(buffer);

	deactivate_lexical_analyzer();

	sprintf(buffer, "Unit Test %zd: Ending Lexical Analizer Unit Tests \n\n", test_step);
	log_generic_message(buffer);

	return all_unit_tests_passed;
}

#ifndef ALL_UNIT_TESTING
int main()
{
	error_out_file = stderr;
	int passed = EXIT_SUCCESS;

	if (!init_vm_error_reporting(NULL) ||
		!init_unit_tests("syntax_state_machine_unit_test_log.txt"))
	{
		return EXIT_FAILURE;
	}

	if (!run_all_syntax_state_machine_unit_tests(0))
	{
		passed = EXIT_FAILURE;
	}

	close_unit_tests();
	disengage_error_reporting();

	return passed;
}
#endif


