#ifndef PARSER_UNIT_TEST_MAIN_C
#define PARSER_UNIT_TEST_MAIN_C

/*
 * parser_unit_test_main.c
 *
 * Main entry point and control for the parser unit test.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ERH_error_reporting.h"
#include "LAH_lexical_analyzer.h"
#include "lexical_analyzer_unit_test_main.h"
#include "internal_parser_tests.h"
#include "parser.h"
#include "parser_unit_test.h"
#include "UTL_unit_test_logging.h"

bool run_all_parser_unit_tests(size_t test_step)
{
	bool all_unit_tests_passed = true;
	UTL_Test_Log_Data* log_data = UTL_new_log_data("run_all_parser_unit_tests", true,
		UTL_POSITIVE_PATH, test_step == 0, false);

	UTL_log_high_level_start_test(log_data, test_step);

	// Make sure all the internal parser functions work before testing the
	// parser.
	UTL_Test_Log_Data* internal_parser_unit_tests = UTL_new_log_data(
		"run_all_internal_parser_unit_tests", true, UTL_POSITIVE_PATH,
		test_step == 0, true);
	all_unit_tests_passed = run_all_internal_parser_unit_tests(
		internal_parser_unit_tests);

	log_data->status = internal_parser_unit_tests->status;

	if (all_unit_tests_passed)
	{
		UTL_log_start_unit_test(log_data, NULL);
		// test the public interface for the parser
		all_unit_tests_passed = unit_test_parser(NULL, test_step);
		log_data->status = all_unit_tests_passed;
		UTL_log_end_unit_test(log_data, NULL);
	}

	deactivate_parser();

	UTL_log_high_level_test_result(log_data, test_step);
	
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

	/*
	 * Lexical analyzer unit tests must pass before testing the parser. The Lexical
	 * analyzer functions are used extensively by the parser.
	 */
	if (!run_all_lexical_analyzer_unit_tests(unit_test_test_step))
	{
		passed = EXIT_FAILURE;
	}
	else
	{
		UTL_va_test_log_formatted_output(NULL, false, "Unit Test %u: Ending Lexical"
			" Analizer Unit Tests %s\n\n",
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
