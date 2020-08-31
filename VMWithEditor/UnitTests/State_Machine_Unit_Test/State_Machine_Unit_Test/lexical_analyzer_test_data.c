#include "common_unit_test_logic.h"
#include "lexical_analyzer_test_data.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void deallocate_test_program(size_t test_program_size, unsigned char **test_program)
{
	if (!test_program)
	{
		return;
	}

	for (size_t i = 0; i < test_program_size; i++)
	{
		free(test_program[i]);
	}

	free(test_program);
}

static void deallocate_expected_data(Expected_Syntax_Errors* expected_errors)
{
	if (!expected_errors)
	{
		return;
	}

	free(expected_errors);
}

void deallocate_lexical_test_data(Lexical_Analyzer_Test_Data* deletee)
{
	if (!deletee)
	{
		return;
	}

	if (deletee->expected_errors)
	{
		deallocate_expected_data(deletee->expected_errors);
		deletee->expected_errors = NULL;
	}

	if (deletee->test_program)
	{
		deallocate_test_program(deletee->test_program_size, deletee->test_program);
		deletee->test_program = NULL;
	}

	free(deletee);
}

void lexical_analyzer_test_data_allocation_failed(Test_Log_Data* log_data, char* allocating_function,
	char* allocation_function)
{
	fprintf(error_out_file, "Memory Allocation Error in %s\n", allocating_function);
	fprintf(error_out_file, "\t%s failed for allocation of test data\n", allocation_function);
	fprintf(error_out_file, "\t Unable to continue %s\n", log_data->function_name);
}

static Lexical_Analyzer_Test_Data* create_and_init_lexical_test_data(unsigned char** test_program,
	size_t test_program_size, Expected_Syntax_Errors* expected_data,
	Test_Log_Data* log_data, char* allocating_function)
{
	Expected_Syntax_Errors* expected_errors_dup = calloc(test_program_size, sizeof(*expected_errors_dup));
	if (!expected_errors_dup)
	{
		lexical_analyzer_test_data_allocation_failed(log_data, "init_positive_path_data_for_lexical_analysis", "calloc");
		return NULL;
	}

	for (size_t step_count = 0; step_count < test_program_size; step_count++)
	{
		expected_errors_dup[step_count].error_count = expected_data[step_count].error_count;
		for (size_t checklist_item = 0; checklist_item < SYNTAX_CHECK_COUNT; checklist_item++)
		{
			expected_errors_dup[step_count].syntax_check_list[checklist_item] = expected_data[step_count].syntax_check_list[checklist_item];
		}
	}

	unsigned char** test_program_dupe = calloc(test_program_size, sizeof(*test_program_dupe));
	if (!test_program_dupe)
	{
		lexical_analyzer_test_data_allocation_failed(log_data, "init_positive_path_data_for_lexical_analysis", "calloc");
		deallocate_expected_data(expected_errors_dup);
		return NULL;
	}

	for (size_t step_count = 0; step_count < test_program_size; step_count++)
	{
		test_program_dupe[step_count] = ucstrdup(test_program[step_count]);
		if (!test_program_dupe[step_count])
		{
			lexical_analyzer_test_data_allocation_failed(log_data, "init_positive_path_data_for_lexical_analysis", "ucstrdup");
			deallocate_test_program(step_count, test_program_dupe);
			deallocate_expected_data(expected_errors_dup);
			return NULL;
		}
	}

	Lexical_Analyzer_Test_Data* new_lexical_test_data = calloc(1, sizeof(*new_lexical_test_data));
	if (!new_lexical_test_data)
	{
		lexical_analyzer_test_data_allocation_failed(log_data, allocating_function, "calloc");
		return NULL;
	}

	new_lexical_test_data->test_program_size = test_program_size;
	new_lexical_test_data->test_program = test_program_dupe;
	new_lexical_test_data->expected_errors = expected_errors_dup;

	return new_lexical_test_data;
}

Lexical_Analyzer_Test_Data* init_positive_path_data_for_lexical_analysis(Test_Log_Data* log_data)
{

	unsigned char* test_program[] = {
	(unsigned char*)"    {PUSH, 0x0A},\n", (unsigned char*)"    {PUSH, 0x43},\n",
	(unsigned char*)"{ PUSH, 0x42 },\n", (unsigned char*)"{ PUSH, 0x41 },\n",
	(unsigned char*)"{ OUTPUTCHAR, 0x00 },    \n", (unsigned char*)"{ POP, 0x00 },    \n",
	(unsigned char*)"{ OUTPUTCHAR, 0x00 },\n", (unsigned char*)"{    POP, 0x00  },\n",
	(unsigned char*)"{OUTPUTCHAR, 0x00},\n", (unsigned char*)"{POP, 0x00},\n",
	(unsigned char*)"{HALT, 0x00}"
	};
	size_t test_size = sizeof(test_program) / sizeof(*test_program);


	Expected_Syntax_Errors* expected_errors = calloc(test_size, sizeof(*expected_errors));
	if (!expected_errors)
	{
		lexical_analyzer_test_data_allocation_failed(log_data, "init_positive_path_data_for_lexical_analysis", "calloc");
		return NULL;
	}

	Expected_Syntax_Errors sample_expect_data = { 0, {1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0} };
	for (size_t test = 0; test < test_size; test++)
	{
		expected_errors[test].error_count = sample_expect_data.error_count;
		for (size_t checklist_item = 0; checklist_item < SYNTAX_CHECK_COUNT; checklist_item++)
		{
			expected_errors[test].syntax_check_list[checklist_item] = sample_expect_data.syntax_check_list[checklist_item];
		}
	}
	expected_errors[test_size - 1].syntax_check_list[COMMA] = 1;

	Lexical_Analyzer_Test_Data* positive_test_data = create_and_init_lexical_test_data(
		test_program, test_size, expected_errors, log_data, "init_positive_path_data_for_lexical_analysis");

	return positive_test_data;
}

Lexical_Analyzer_Test_Data* init_negative_path_data_for_lexical_analysis(Test_Log_Data* log_data)
{
	unsigned char* test_program[] =
	{
		(unsigned char*)"    {PUSH, 0x0A},\n",							// No problem
		(unsigned char*)"    PUSH, 0x43},\n",							// Missing open brace
		(unsigned char*)"{ PUSH, 0x42 ,\n",								// Missing close brace
		(unsigned char*)" { PUSH, 0x41 }, { OUTPUTCHAR 0x00 },    \n",	// Multiple statements on one line missing comma in second statement
		(unsigned char*)"{ , 0x00 },    \n",							// Missibg opcode
		(unsigned char*)"{ OUTPUTCHAR 0x00 },",							// Missing comma between opcode and operand
		(unsigned char*)"{    POP,  },",								// Missing OPERAND
		(unsigned char*)"{OUTPUTCHAR, 0x00}\n",							// Missing last comma
		(unsigned char*)"{POP, 0x00 POP},\n",							// Illegal characters after operand
		(unsigned char*)"HALT"											// Missing open brace, commas, operand, close brace
	};
	size_t test_size = sizeof(test_program) / sizeof(*test_program)
		;
	Expected_Syntax_Errors expected_errors[] =
	{
		{0, {1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0}},			// {PUSH, 0x0A}, No problem
		{2, {0, 1, 2, 1, 1, 0, 0, 1, 0, 0, 0}},			// PUSH, 0x43}, Missing open brace and Illegal character in first column
		{1, {1, 0, 2, 1, 1, 0, 0, 0, 0, 0, 0}},			// { PUSH, 0x42 , Missing close brace
		{1, {2, 1, 2, 1, 1, 0, 0, 0, 1, 0, 0}},			// { PUSH, 0x41 }, { OUTPUTCHAR 0x00 }, Multiple statements on one line missing comma in second statement
		{1, {1, 1, 2, 0, 1, 0, 0, 0, 0, 0, 0}},			// { , 0x00 }, Missibg opcode
		{1, {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1}},			// { OUTPUTCHAR 0x00 }, Missing comma between opcode and operand
		{1, {1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0}},			// {    POP,  }, Missing OPERAND
		{0, {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}},			// {OUTPUTCHAR, 0x00}, Missing last comma
		{1, {1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0}},			// {POP, 0x00 POP}, Illegal characters after operand
		{5, {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}}			// HALT Missing open brace, commas, operand, close brace
	};

	Lexical_Analyzer_Test_Data* negative_test_data = create_and_init_lexical_test_data(
		test_program, test_size, expected_errors, log_data, "init_negative_path_data_for_lexical_analysis");

	return negative_test_data;
}
