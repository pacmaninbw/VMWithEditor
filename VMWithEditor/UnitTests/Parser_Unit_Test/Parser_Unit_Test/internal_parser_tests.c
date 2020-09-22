/*
 * internal_parser_tests.c
 *
 * This file includes parser.c because there is unit testing of static
 * functions and variables. Because this this file is included by parser.c
 * include files are not necessary.
 */

#ifndef INTERNAL_PARSER_TESTS_C
#define INTERNAL_PARSER_TESTS_C

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "ERH_error_reporting.h"
#include "HRF_human_readable_program_format.h"
#include "LAH_lexical_analyzer.h"
#include "UTL_unit_test_logging.h"
#include "internal_parser_tests.h"

/*
 * Include parser.c so we can test the internal static functions in the file.
 */
#include "parser.c"

static bool unit_test_get_opcode_or_operand_string(size_t test_step)
{
	bool passed = true;
	UTL_Test_Log_Data *log_data = UTL_new_log_data(
		"unit_test_get_opcode_or_operand_string", passed, UTL_POSITIVE_PATH,
		test_step == 0, true);

	UTL_log_start_unit_test(log_data, NULL);

	UTL_log_test_not_implemented(log_data);
	log_data->status = false;

	if (passed)
	{
		passed = log_data->status;
	}

	UTL_log_end_unit_test(log_data, NULL);

	return passed;
}

static bool unit_test_get_legal_opcode_or_oparand(size_t test_step)
{
	bool passed = true;
	UTL_Test_Log_Data* log_data = UTL_new_log_data(
		"unit_test_get_legal_opcode_or_oparand", passed, UTL_POSITIVE_PATH,
		test_step == 0, true);

	UTL_log_start_unit_test(log_data, NULL);

	UTL_log_test_not_implemented(log_data);
	log_data->status = false;

	if (passed)
	{
		passed = log_data->status;
	}

	UTL_log_end_unit_test(log_data, NULL);

	return passed;
}

static bool unit_test_print_syntax_errors(size_t test_step)
{
	bool passed = true;
	unsigned syntax_check_list[LAH_SYNTAX_CHECK_ARRAY_SIZE];
	char* test_file_name = "No File Name";
	char* text_line = "Unit Testing print_syntax_errors()";
	bool this_test_passed = true;
	UTL_Test_Log_Data* log_data = UTL_new_log_data(
		"unit_test_print_syntax_errors", passed, UTL_POSITIVE_PATH, (!test_step), true);
	size_t line_number = 1;

	UTL_log_start_unit_test(log_data, NULL);
	UTL_log_start_test_path(log_data, NULL);

	for (size_t i = 0; i < LAH_SYNTAX_CHECK_ARRAY_SIZE; i++)
	{
		if (i < LAH_ILLEGALOPCODE)
		{
			syntax_check_list[i] = 1;
		}
		else
		{
			syntax_check_list[i] = 0;
		}
	}

	this_test_passed = print_syntax_errors(syntax_check_list, &line_number, test_file_name, (unsigned char*)text_line);
	UTL_log_test_status_each_step(log_data);
	passed = (!this_test_passed) ? false : passed;
	UTL_log_end_test_path(log_data, NULL);

	log_data->path = UTL_NEGATIVE_PATH;
	UTL_log_start_test_path(log_data, NULL);
	/*
	 * The Ultimate negative test path, every check should fail.
	 */
	for (size_t i = 0; i < LAH_SYNTAX_CHECK_ARRAY_SIZE; i++)
	{
		if (i < LAH_ILLEGALOPCODE)
		{
			syntax_check_list[i] = 0;
		}
		else
		{
			syntax_check_list[i] = 1;
		}
	}

	this_test_passed = !print_syntax_errors(syntax_check_list, &line_number, test_file_name, (unsigned char*)text_line);
	UTL_log_test_status_each_step(log_data);
	passed = (!this_test_passed) ? false : passed;
	log_data->status = this_test_passed;
	UTL_log_end_test_path(log_data, NULL);

	return passed;
}

typedef bool (*parser_unit_test_function)(size_t test_step);

typedef struct unit_test_functions_and_args
{
	char* test_name;
	parser_unit_test_function func;
} Parser_Unit_Test_Functions;


bool run_all_internal_parser_unit_tests(size_t test_step)
{
	bool all_tests_passed = true;
	UTL_Test_Log_Data* log_data = UTL_new_log_data("run_all_internal_parser_unit_tests", all_tests_passed, UTL_POSITIVE_PATH, test_step == 0, true);

	Parser_Unit_Test_Functions unit_tests[] =
	{
		{"unit_test_get_opcode_or_operand_string",
			unit_test_get_opcode_or_operand_string},
		{"unit_test_get_legal_opcode_or_oparand",
			unit_test_get_legal_opcode_or_oparand},
		{"unit_test_print_syntax_errors",
			unit_test_print_syntax_errors},
	};
	size_t test_max = (sizeof(unit_tests) / sizeof(*unit_tests));

	const char* overall_log_function_name = log_data->function_name;
	for (size_t test_count = 0; test_count < test_max; test_count++)
	{
		bool test_passed = unit_tests[test_count].func(test_step);
		log_data->function_name = unit_tests[test_count].test_name;
		log_data->status = test_passed;
		UTL_log_high_level_test_result(log_data, test_count + 1);
		// if one test already failed we are good
		if (all_tests_passed)
		{
			all_tests_passed = test_passed;
		}
	}

	return all_tests_passed;
}

#endif // !INTERNAL_PARSER_TESTS_C
