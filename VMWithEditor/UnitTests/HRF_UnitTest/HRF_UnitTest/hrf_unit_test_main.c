/*
 * Run the unit tests for the Human Readable Program portion of the VM with Editor
 */

#ifndef HRF_UNIT_TEST_MAIN_C
#define HRF_UNIT_TEST_MAIN_C

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "ERH_error_reporting.h"
#include "HRF_human_readable_program_format.h"
#include "hrf_unit_test_main.h"
#include "unit_test_human_readable_program_format.h"
#include "UTL_unit_test_logging.h"

typedef bool (*unit_test_function2)(char* file_name, size_t test_step);
typedef bool (*unit_test_function1)(size_t test_step);

typedef union {
	unit_test_function1 func1;
	unit_test_function2 func2;
} unit_test_function;

typedef struct unit_test_functions_and_args
{
	unsigned arg_count;
	char* test_name;
	unit_test_function func;
} Unit_Test_Functions_and_Args;

static bool hrf_individual_stand_alone_unit_tests(size_t test_max, Unit_Test_Functions_and_Args unit_tests[])
{
	bool passed = true;
	size_t test_step = 0;
	char* file_name = NULL;
	UTL_Test_Log_Data* log_data = UTL_new_log_data(
		"hrf_individual_stand_alone_unit_tests", passed, UTL_POSITIVE_PATH, true, false);

	for (size_t test_count = 0; test_count < test_max; test_count++)
	{
		bool test_passed = (unit_tests[test_count].arg_count == 1) ?
			unit_tests[test_count].func.func1(test_step) :
			unit_tests[test_count].func.func2(file_name, test_step);
		UTL_log_high_level_test_result(log_data, test_count + 1);
		passed = test_passed;
	}

	return passed;
}

bool run_all_hrf_unit_tests(size_t test_id)
{
	bool all_unit_tests_passed = true;
	UTL_Test_Log_Data* log_data = UTL_new_log_data(
		"run_all_hrf_unit_tests", all_unit_tests_passed, UTL_POSITIVE_PATH,
		true, false);

	Unit_Test_Functions_and_Args unit_tests[] =
	{
		{1, "unit_test_hrf_duplicate_program", unit_test_hrf_duplicate_program},
		{1, "unit_test_hrf_create_program_step", unit_test_hrf_create_program_step},
		{1, "unit_test_hrf_convert_array_program_to_linked_list",
			unit_test_hrf_convert_array_program_to_linked_list},
		{1, "unit_test_hrf_convert_link_list_program_to_array",
			unit_test_hrf_convert_link_list_program_to_array},
	};

	size_t test_count = (sizeof(unit_tests) / sizeof(*unit_tests));

	if (hrf_individual_stand_alone_unit_tests(test_count, unit_tests))
	{
		test_count++;
		all_unit_tests_passed = unit_test_all_human_readable_format();
	}
	else
	{
		all_unit_tests_passed = false;
	}

	log_data->status = all_unit_tests_passed;
	UTL_log_high_level_test_result(log_data, test_id);

	free(log_data);

	return all_unit_tests_passed;
}

#ifdef HRF_UNIT_TESTING_ONLY
/*
 * All the unit tests can be combined into a single executable, when this is
 * done main should not be in each file.
 */
int main()
{
	ERH_error_out_file = stderr;
	int passed = EXIT_SUCCESS;

	if (!ERH_init_vm_error_reporting(NULL) ||
		!UTL_init_unit_tests("human_readable_format_unit_test_log.txt"))
	{
		return EXIT_FAILURE;
	}

	if (!run_all_hrf_unit_tests(0))
	{
		passed = EXIT_FAILURE;
	}

	UTL_close_unit_tests();
	ERH_disengage_error_reporting();

	return passed;
}
#endif	// HRF_UNIT_TESTING_ONLY

#endif // !HRF_UNIT_TEST_MAIN_C
