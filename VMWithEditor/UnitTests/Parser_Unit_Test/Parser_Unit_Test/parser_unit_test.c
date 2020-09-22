#ifndef PARSER_UNIT_TEST_C
#define PARSER_UNIT_TEST_C

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "parser.h"
#include "ERH_error_reporting.h"
#include "UTL_unit_test_logging.h"
#include "HRF_human_readable_program_format.h"
#include "OPC_opcode.h"

typedef struct syntax_strings_for_testing
{
	char* string_to_check;
	UTL_Path_State path;
} Syntax_Strings_For_Testing;

static Syntax_Strings_For_Testing* init_syntax_test_data(size_t* test_data_size, size_t* positive_count)
{
	Syntax_Strings_For_Testing check_line_syntax_test_data[] =
	{
		{"{HALT, 0x0}", UTL_POSITIVE_PATH},
		{"{HALT, 0x0},", UTL_POSITIVE_PATH},
		{"{HALT 0x0},", UTL_NEGATIVE_PATH},					// missing comma
		{"{LOAD, 0x0}, {HALT, 0x0},", UTL_NEGATIVE_PATH},	// 1 per line
		{"HALT, 0x0},", UTL_NEGATIVE_PATH},					// missing opening brace
		{"{HALT, 0x0,", UTL_NEGATIVE_PATH},					// missing closing brace
		{"{, 0x0}", UTL_NEGATIVE_PATH},						// missing opcode
		{"{ 0x0}", UTL_NEGATIVE_PATH},						// missing opcoe and comma
		{"{HALT, }", UTL_NEGATIVE_PATH},					// missing operand
		{"{HALT}", UTL_NEGATIVE_PATH}						// missing comma and operand
	};

	*test_data_size = sizeof(check_line_syntax_test_data) / sizeof(*check_line_syntax_test_data);

	Syntax_Strings_For_Testing* test_data = calloc(*test_data_size, sizeof(*test_data));
	if (test_data)
	{
		for (size_t i = 0; i < *test_data_size; i++)
		{
			test_data[i] = check_line_syntax_test_data[i];
			if (test_data[i].path == UTL_POSITIVE_PATH)
			{
				++(*positive_count);
			}
		}
	}
	else
	{
		ERH_va_report_error_fprintf("Calloc failed in init_syntax_test_data()\n");
	}

	return test_data;
}

/*
 * If we are testing the positive path, then the parser should return a completed
 * HRF_Program_Step_Node because the statement parsed properly, if we are testing
 * the negative path, the parser should encounter error syntax and return NULL.
 */
static bool single_syntax_test(UTL_Test_Log_Data *log_data, Syntax_Strings_For_Testing test_data, char* test_file_name)
{
	bool this_test_passed = true;
	HRF_Program_Step_Node* test_head = NULL;
	size_t prog_size = 1;

	test_head = parser((unsigned char*)test_data.string_to_check, &prog_size, test_file_name);
	this_test_passed = test_data.path == UTL_POSITIVE_PATH ?
		(test_head && test_head->opcode_and_operand.opcode == OPC_HALT) :
		(!test_head);
	log_data->status = this_test_passed;
	UTL_log_test_status_each_step(log_data);
	free(test_head);
	UTL_va_test_log_formatted_output(NULL, false, "\n");

	return this_test_passed;
}

bool unit_test_parser(char* file_name, unsigned test_step)
{
	bool all_tests_passed = true;
	bool stand_alone_test = (!file_name && !test_step);
	UTL_Test_Log_Data* log_data = UTL_new_log_data("unit_test_parser",
		all_tests_passed, UTL_POSITIVE_PATH, stand_alone_test, true);
	char* test_file_name = NULL;
	size_t test_data_size = 0;
	size_t positive_count = 0;
	Syntax_Strings_For_Testing* test_data = init_syntax_test_data(&test_data_size, &positive_count);
	if (!test_data)
	{
		return false;
	}

	test_file_name = (!file_name || !test_step) ? "No File Name" : file_name;
	UTL_log_start_unit_test(log_data, NULL);
	UTL_log_start_test_path(log_data, NULL);

	size_t test_count = 0;
	for (; test_count < positive_count; test_count++)
	{
		bool passed = single_syntax_test(log_data, test_data[test_count], test_file_name);
		if (all_tests_passed)
		{
			all_tests_passed = passed;
		}
	}

	UTL_log_end_test_path(log_data, NULL);
	log_data->path = UTL_NEGATIVE_PATH;
	UTL_log_start_test_path(log_data, NULL);

	for (; test_count < test_data_size; test_count++)
	{
		bool passed = single_syntax_test(log_data, test_data[test_count], test_file_name);
		if (all_tests_passed)
		{
			all_tests_passed = passed;
		}
	}

	log_data->status = all_tests_passed;
	UTL_log_end_test_path(log_data, NULL);
	UTL_log_end_unit_test(log_data, NULL);

	free(test_data);
	free(log_data);

	return all_tests_passed;
}

#endif	// !PARSER_UNIT_TEST_C