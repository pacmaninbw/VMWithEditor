#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Editor_UnitTest_main.h"

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
#if 0
	error_out_file = stderr;

	if (!init_vm_error_reporting(NULL) || !init_hrf_unit_tests("human_readable_format_unit_test_log.txt"))
	{
		return EXIT_FAILURE;
	}

	if (!run_all_hrf_unit_tests())
	{
		passed = EXIT_FAILURE;
	}

	close_hrf_unit_tests();
	disengage_error_reporting();
#endif

	return passed;

}
#endif


