#ifndef PARSER_UNIT_TEST_MAIN_C
#define PARSER_UNIT_TEST_MAIN_C
#include <stdbool.h>
//#include <stdio.h>
#include <stdlib.h>

#include "lexical_analyzer_unit_test_main.h"
#include "error_reporting.h"
#include "unit_test_logging.h"

bool run_all_parser_unit_tests(unsigned test_step)
{
	bool all_unit_tests_passed = true;
	char buffer[BUFSIZ];

	sprintf(buffer, "Unit Test %u: Starting Lexical Analizer Unit Tests \n\n", test_step);
	log_generic_message(buffer);

	all_unit_tests_passed = internal_tests_on_all_state_transitions(test_step);

	if (all_unit_tests_passed)
	{
		// test the public interface for the lexical analyzer
		all_unit_tests_passed =
			unit_test_parser(test_step);
	}

	sprintf(buffer, "Unit Test %u: run_all_parser_unit_tests(unsigned test_step) : %s\n\n",
		test_step, all_unit_tests_passed ? "Passed" : "Failed");
	log_generic_message(buffer);

	deactivate_lexical_analyzer();

	sprintf(buffer, "Unit Test %u: Ending Lexical Analizer Unit Tests \n\n", test_step);
	log_generic_message(buffer);

	return all_unit_tests_passed;
}

#ifdef PARSER_UNIT_TEST_ONLY
int main()
{
	error_out_file = stderr;
	int passed = EXIT_SUCCESS;

	if (!init_vm_error_reporting(NULL) ||
		!init_unit_tests("parser_unit_test_log.txt"))
	{
		return EXIT_FAILURE;
	}

	// Lexical analyzer unit tests must pass before testing the parser.
	if (!run_all_lexical_analyzer_unit_tests(0))
	{
		passed = EXIT_FAILURE;
	}

	if (passed == EXIT_SUCCESS)
	{
		if (!run_all_parser_unit_tests(0))
		{
			passed = EXIT_FAILURE;
		}
	}

	close_unit_tests();
	disengage_error_reporting();

	return passed;
}
#endif  // PARSER_UNIT_TEST_ONLY

#endif	// PARSER_UNIT_TEST_MAIN_C
