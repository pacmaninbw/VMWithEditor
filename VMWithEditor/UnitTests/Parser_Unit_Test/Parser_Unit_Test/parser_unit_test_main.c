#ifndef PARSER_UNIT_TEST_MAIN_C
#define PARSER_UNIT_TEST_MAIN_C
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "LAH_lexical_analyzer.h"
#include "lexical_analyzer_unit_test_main.h"
#include "ERH_error_reporting.h"
#include "internal_sytax_state_tests.h"
#include "UTL_unit_test_logging.h"
#include "internal_parser_tests.h"
#include "parser_unit_test.h"

bool run_all_parser_unit_tests(size_t test_step)
{
	bool all_unit_tests_passed = true;

	// Make sure all the internal parser functions work before testing the
	// parser.
	all_unit_tests_passed = run_all_internal_parser_unit_tests(test_step);
	UTL_va_log_fprintf("Unit Test %u: Ending Internal Parser Unit Tests : %s\n\n",
		test_step, all_unit_tests_passed ? "Passed" : "Failed");

	if (all_unit_tests_passed)
	{
		// test the public interface for the parser
		all_unit_tests_passed = unit_test_parser(NULL, test_step);
	}

	UTL_va_log_fprintf("Unit Test %u: run_all_parser_unit_tests(unsigned test_step) : %s\n\n",
		test_step, all_unit_tests_passed ? "Passed" : "Failed");

	deactivate_parser();

	UTL_va_log_fprintf("Unit Test %u: Ending Parser Unit Tests \n\n", test_step);

	return all_unit_tests_passed;
}

/*
 * The Parser Unit Test is included in other unit tests such as the RunAllUnitTests,
 * the main() function is only needed when the parser is built as a stand alone
 * application.
 */
#ifdef PARSER_UNIT_TEST_ONLY
int main()
{
	ERH_error_out_file = stderr;
	int passed = EXIT_SUCCESS;
	const size_t unit_test_test_step = 1;

	if (!ERH_init_vm_error_reporting(NULL) ||
		!UTL_init_unit_tests("parser_unit_test_log.txt"))
	{
		return EXIT_FAILURE;
	}

	// Lexical analyzer unit tests must pass before testing the parser.
	if (!run_all_lexical_analyzer_unit_tests(unit_test_test_step))
	{
		passed = EXIT_FAILURE;
	}
	else
	{
		UTL_va_log_fprintf("Unit Test %u: Ending Lexical Analizer Unit Tests %s\n\n",
			unit_test_test_step, "Parser ready for testing.");
	}

	if (passed == EXIT_SUCCESS)
	{
		if (!run_all_parser_unit_tests(0))
		{
			passed = EXIT_FAILURE;
		}
	}

	UTL_close_unit_tests();
	ERH_disengage_error_reporting();

	return passed;
}
#endif  // PARSER_UNIT_TEST_ONLY

#endif	// PARSER_UNIT_TEST_MAIN_C
