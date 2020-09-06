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

#include "arg_flags.h"
#include "error_reporting.h"
#include "unit_test_logging.h"

bool run_all_command_line_unit_tests(void)
{
	bool all_unit_tests_passed = true;

	char buffer[LOG_BUFFER_SIZE];


	return all_unit_tests_passed;
}

#ifdef COMMANDLINE_UNIT_TESTING_ONLY
int main()
{
	error_out_file = stderr;
	int passed = EXIT_SUCCESS;

	if (!init_vm_error_reporting(NULL) ||
		!init_unit_tests("commandline_unittest_log.txt"))
	{
		return EXIT_FAILURE;
	}

	if (!run_all_command_line_unit_tests())
	{
		passed = EXIT_FAILURE;
	}

	close_unit_tests();
	disengage_error_reporting();

	return passed;
}
#endif	// COMMANDLINE_UNIT_TESTING_ONLY

#endif	// !COMMANDLINE_UNITTEST_C
