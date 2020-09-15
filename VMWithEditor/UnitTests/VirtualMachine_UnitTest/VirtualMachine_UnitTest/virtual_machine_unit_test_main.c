#ifndef VIRTUAL_MACHINE_UNIT_TEST_MAIN_C
#define VIRTUAL_MACHINE_UNIT_TEST_MAIN_C
/*
 * virtual_machine_unit_test_main.c
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ERH_error_reporting.h"
#include "UTL_unit_test_logging.h"
#include "virtual_machine_unit_test_main.h"

bool run_all_vm_unit_tests(void)
{
	bool all_tests_pased = true;

	printf("Hello World!\n");

	return all_tests_pased;
}

#ifdef VIRTUAL_MACHINE_UNIT_TEST_ONLY
/*
 * All the unit tests can be combined into a single executable, when this is
 * done main should not be in each file.
 */
int main()
{
	ERH_error_out_file = stderr;
	int passed = EXIT_SUCCESS;

	if (!ERH_init_vm_error_reporting(NULL) ||
		!UTL_init_unit_tests("virtual_machine_unit_test_log.txt"))
	{
		return EXIT_FAILURE;
	}

	if (!run_all_vm_unit_tests())
	{
		passed = EXIT_FAILURE;
	}

	UTL_close_unit_tests();
	ERH_disengage_error_reporting();

	return passed;
}
#endif	// VIRTUAL_MACHINE_UNIT_TEST_ONLY

#endif // !VIRTUAL_MACHINE_UNIT_TEST_MAIN_C
