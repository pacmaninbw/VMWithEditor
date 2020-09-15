#ifndef COMMANDLINE_UNITTEST_C
#define COMMANDLINE_UNITTEST_C
/*
 * CommandLine_UnitTest.c :
 * This file contains the 'main' function. Program execution begins and ends there.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ARGF_arg_flags.h"
#include "command_line_unit_test_main.h"
#include "ERH_error_reporting.h"
#include "UTL_unit_test_logging.h"

bool run_all_command_line_unit_tests(const size_t test_step)
{
	bool all_unit_tests_passed = true;

	return all_unit_tests_passed;
}

/*
 * The Command Line Unit Test is included in other unit tests such as the
 * RunAllUnitTests, the main() function is only needed when the command line
 * unit test is built as a stand alone application.
 */
#ifdef COMMANDLINE_UNIT_TESTING_ONLY
int main()
{
	ERH_error_out_file = stderr;
	int passed = EXIT_SUCCESS;

	if (!ERH_init_vm_error_reporting(NULL) ||
		!UTL_init_unit_tests("commandline_unittest_log.txt"))
	{
		return EXIT_FAILURE;
	}

	if (!run_all_command_line_unit_tests(0))
	{
		passed = EXIT_FAILURE;
	}

	UTL_close_unit_tests();
	ERH_disengage_error_reporting();

	return passed;
}
#endif	// COMMANDLINE_UNIT_TESTING_ONLY

#endif	// !COMMANDLINE_UNITTEST_C
