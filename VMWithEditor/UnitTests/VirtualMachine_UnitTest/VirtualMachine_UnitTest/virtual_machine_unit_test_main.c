#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "virtual_machine_unit_test_main.h"

bool run_all_vm_unit_tests(void)
{
	printf("Hello World!\n");
}

#ifndef ALL_UNIT_TESTING
int main()
{
	error_out_file = stderr;
	int passed = EXIT_SUCCESS;

	if (!init_vm_error_reporting(NULL) || !init_hrf_unit_tests("human_readable_format_unit_test_log.txt"))
	{
		return EXIT_FAILURE;
	}

	if (!run_all_vm_unit_tests())
	{
		passed = EXIT_FAILURE;
	}

	close_hrf_unit_tests();
	disengage_error_reporting();

	return passed;
}
#endif

