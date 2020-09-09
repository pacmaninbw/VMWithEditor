#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Editor_UnitTest_main.h"
#include "ERH_error_reporting.h"
#include "UTL_unit_test_logging.h"

bool run_all_editor_unit_tests(void)
{
	bool all_unit_tests_passed = true;

	printf("Hello World!\n");

	return all_unit_tests_passed;
}

#ifdef EDITOR_UNIT_TEST_ONLY
int main()
{
	int passed = EXIT_SUCCESS;

	ERH_error_out_file = stderr;

	if (!ERH_init_vm_error_reporting(NULL) ||
		UTL_init_unit_tests("editor_unit_test_log.txt"))
	{
		return EXIT_FAILURE;
	}

	if (!run_all_editor_unit_tests())
	{
		passed = EXIT_FAILURE;
	}

	UTL_close_unit_tests();
	ERH_disengage_error_reporting();

	return passed;

}
#endif


