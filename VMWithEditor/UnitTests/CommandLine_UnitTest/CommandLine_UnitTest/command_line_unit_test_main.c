// CommandLine_UnitTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "common_unit_test_logic.h"
#include "arg_flags.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool run_all_command_line_unit_tests(void)
{
	bool all_unit_tests_passed = true;

	char buffer[BUFSIZ];


	return all_unit_tests_passed;
}

#ifndef ALL_UNIT_TESTING
int main()
{
	error_out_file = stderr;
	int passed = EXIT_SUCCESS;

	if (!init_vm_error_reporting(NULL))
	{
		return EXIT_FAILURE;
	}

	if (!run_all_command_line_unit_tests())
	{
		passed = EXIT_FAILURE;
	}

	disengage_error_reporting();

	return passed;
}
#endif

