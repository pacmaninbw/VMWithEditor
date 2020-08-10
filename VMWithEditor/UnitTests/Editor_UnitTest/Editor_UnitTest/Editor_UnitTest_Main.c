#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Editor_UnitTest_main.h"
#ifndef UNIT_TESTING
#include "common_program_logic.h"
#else
#include "common_unit_test_logic.h"
#endif

bool run_all_editor_unit_tests(void)
{
	bool all_unit_tests_passed = true;

	printf("Hello World!\n");

	return all_unit_tests_passed;
}

#ifndef ALL_UNIT_TESTING
int main()
{
	int passed = EXIT_SUCCESS;

	error_out_file = stderr;

	if (!init_vm_error_reporting(NULL))
	{
		return EXIT_FAILURE;
	}

#if 0
	if (!run_all_hrf_unit_tests())
	{
		passed = EXIT_FAILURE;
	}

	close_hrf_unit_tests();
#endif
	disengage_error_reporting();

	return passed;

}
#endif


