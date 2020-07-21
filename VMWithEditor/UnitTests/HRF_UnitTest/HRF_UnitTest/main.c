#include "human_readable_program_format.h"
#include "common_program_logic.h"
#include <stdio.h>
#include <stdlib.h>

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

bool individual_stand_alone_unit_tests(size_t test_max, Unit_Test_Functions_and_Args unit_tests[])
{
	bool passed = true;
	size_t test_step = 0;
	char* file_name = NULL;
	char buffer[BUFSIZ];

	for (size_t test_count = 0; test_count < test_max && passed; test_count++)
	{
		bool test_passed = (unit_tests[test_count].arg_count == 1)?
			unit_tests[test_count].func.func1(test_step) :
			unit_tests[test_count].func.func2(file_name, test_step);
		sprintf(buffer, "\nUnit Test %zd: %s : %s\n\n", test_count + 1, unit_tests[test_count].test_name, (test_passed)? "Passed" : "Failed");
		write_to_log_file(buffer);
		passed = test_passed;
	}

	return passed;
}

int main()
{
	error_out_file = stderr;
	int passed = EXIT_SUCCESS;
	char buffer[BUFSIZ];
	Unit_Test_Functions_and_Args unit_tests[] =
	{
		{1, "unit_test_hrf_duplicate_program", unit_test_hrf_duplicate_program},
		{1, "unit_test_hrf_create_program_step", unit_test_hrf_create_program_step},
		{1, "unit_test_hrf_convert_link_list_program_to_array", unit_test_hrf_convert_link_list_program_to_array},
		{2, "unit_test_hrf_writing_to_file", unit_test_hrf_writing_to_file},
		{2, "unit_test_hrf_reading_from_file", unit_test_hrf_reading_from_file}
	};

	size_t test_count = (sizeof(unit_tests) / sizeof(*unit_tests));

	if (!init_vm_error_reporting(NULL) || !init_hrf_unit_tests("human_readable_format_unit_test_log.txt"))
	{
		return EXIT_FAILURE;
	}

	if (individual_stand_alone_unit_tests(test_count, unit_tests))
	{
		test_count++;
		if (!unit_test_all_human_readable_format())
		{
			sprintf(buffer, "Unit Test %zd: unit_test_all_human_readable_format() : Failed\n\n", test_count);
			write_to_log_file(buffer);
			passed = EXIT_FAILURE;
		}
		else
		{
			sprintf(buffer, "Unit Test %zd: unit_test_all_human_readable_format() : Passed\n\n", test_count);
			write_to_log_file(buffer);
		}
	}
	else
	{
		passed = EXIT_FAILURE;
	}

	close_hrf_unit_tests();
	disengage_error_reporting();

	return passed;
}

