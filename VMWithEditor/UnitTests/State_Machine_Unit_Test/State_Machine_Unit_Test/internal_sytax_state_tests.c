/* 
 * internal_sytax_state_tests.c
 *
 * This file contains internal lexical analyzer unit tests. These test functions
 * test the very basic functions that are the building blocks of the public
 * interface. The functions being tested are declared static in lexical_analyzer.c
 * so lexical_analyzer.c is included by this file to get access to these functions.
 */


#ifndef INTERNAL_SYNTAX_STATE_TESTS_C
#define INTERNAL_SYNTAX_STATE_TESTS_C
#define INCLUDED_IN_UNIT_TEST

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "LAH_lexical_analyzer.h"
#include "lexical_analyzer.c"
#include "internal_sytax_state_tests.h"
#include "internal_character_transition_unit_tests.h"
#include "lexical_analyzer_test_data.h"
#include "lexical_analyzer_unit_test_utilities.h"

typedef struct state_test_data
{
	LAH_Syntax_State current_state;
	LAH_State_Transition_Characters input_character_state;
	unsigned syntax_items_checklist[LAH_SYNTAX_CHECK_ARRAY_SIZE];
	Expected_Syntax_Errors expected_data;
} Error_Reporting_Test_Data;

#ifdef UNIT_TEST_DEBUG
static bool unit_test_syntax_states(size_t test_step)
{
	bool test_passed = true;
	bool stand_alone = test_step == 0;

	LAH_Syntax_State_Transition* test_transitions = get_or_create_next_states();
	if (!test_transitions)
	{
		fprintf(error_out_file, "Memory allocation error in get_create_next_states()\n");
		return false;
	}

	for (size_t state = 0; state < LAH_SYNTAX_STATE_ARRAY_SIZE; state++)
	{
		if (stand_alone)
		{
			UTL_va_log_fprintf("current_state = %s\n", state_name_for_printing(
				test_transitions[state].current_state));
		}

		if (stand_alone)
		{
			for (size_t character_index = 0; character_index < LAH_TRANSITION_ARRAY_SIZE;
				character_index++)
			{
				UTL_va_log_fprintf("\ttransition character = %s\t\tnew state %s\n",
					transition_names(character_index),
					state_name_for_printing(
						test_transitions[state].transition_on_char_type[character_index]));
			}
			UTL_va_log_fprintf("\n");
		}
	}

	return test_passed;
}
#endif

static void print_syntax_error_checklist(unsigned syntax_checklist[], char *out_buffer, size_t buffer_size)
{
	for (size_t i = 0; i < LAH_SYNTAX_CHECK_ARRAY_SIZE; i++)
	{
		char num_buff[8];
		if (i < LAH_SYNTAX_CHECK_ARRAY_SIZE - 1)
		{
			snprintf(num_buff, sizeof(num_buff), "%u, ", syntax_checklist[i]);
		}
		else
		{
			snprintf(num_buff, sizeof(num_buff), "%u} ", syntax_checklist[i]);
			SSF_strcat(out_buffer, num_buff, buffer_size);
		}
	}
}

static void log_all_failure_data_for_unit_test_collect_error_reporting_data(
	UTL_Test_Log_Data* log_data, Error_Reporting_Test_Data test_data, unsigned syntax_check_list[])
{
	UTL_log_test_status_each_step2(log_data);

	char out_buffer[UTL_LOG_BUFFER_SIZE];
	snprintf(out_buffer, UTL_LOG_BUFFER_SIZE, "\tcurrent_state = %s ", state_name_for_printing(test_data.current_state));
	SSF_strcat(out_buffer, "expected Checklist Values {", UTL_LOG_BUFFER_SIZE);
	print_syntax_error_checklist(test_data.expected_data.syntax_check_list, out_buffer, sizeof(out_buffer));
	SSF_strcat(out_buffer, "new checklist value {", UTL_LOG_BUFFER_SIZE);
	print_syntax_error_checklist(syntax_check_list, out_buffer, sizeof(out_buffer));
	SSF_strcat(out_buffer, "\n", UTL_LOG_BUFFER_SIZE);
	UTL_va_log_fprintf(out_buffer);
}

static bool errors_in_sync(unsigned syntax_check_list[], Expected_Syntax_Errors expected_errors)
{
	bool syntax_check_list_in_sync = true;

	for (size_t i = 0; i < LAH_SYNTAX_CHECK_ARRAY_SIZE; i++)
	{
		if (syntax_check_list[i] != expected_errors.syntax_check_list[i])
		{
			syntax_check_list_in_sync = false;
		}
	}

	return syntax_check_list_in_sync;
}

static bool run_error_checking_unit_tests(
	UTL_Test_Log_Data *log_data, size_t positive_path_test_count,
	Error_Reporting_Test_Data test_data[], size_t test_runs)
{
	bool test_passed = true;

	UTL_log_start_test_path(log_data);

	for (size_t test_count = 0; test_count < test_runs; test_count++)
	{
		log_data->status = true;
		if (test_count == positive_path_test_count)
		{
			UTL_log_end_test_path(log_data);
			log_data->path = UTL_NEGATIVE_PATH;
			UTL_log_start_test_path(log_data);
		}

		unsigned syntax_check_list[LAH_SYNTAX_CHECK_ARRAY_SIZE];
		memcpy(&syntax_check_list[0], &test_data[test_count].syntax_items_checklist[0], sizeof(syntax_check_list));

		collect_error_reporting_data(test_data[test_count].current_state,
			test_data[test_count].input_character_state, syntax_check_list);
		if (!errors_in_sync(syntax_check_list, test_data[test_count].expected_data))
		{
			log_data->status = false;
			log_all_failure_data_for_unit_test_collect_error_reporting_data(
				log_data, test_data[test_count], syntax_check_list);
		}
		else
		{
			UTL_log_test_status_each_step2(log_data);
		}

		if (!log_data->status && test_passed)
		{
			test_passed = log_data->status;
		}
	}

	UTL_log_end_test_path(log_data);

	return test_passed;
}

static Error_Reporting_Test_Data* init_error_report_data(size_t *positive_path_test_count, size_t *test_data_size)
{
	Error_Reporting_Test_Data static_global_test_data[] =
	{
		// Start with positive test path data
		{LAH_START_STATE, LAH_OPENBRACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{LAH_OPERAND_STATE, LAH_CLOSEBRACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{LAH_END_STATEMENT_STATE, LAH_COMMA_STATE_TRANSITION, {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{LAH_OPCODE_STATE, LAH_COMMA_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{LAH_END_OPCODE_STATE, LAH_COMMA_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{LAH_END_OPCODE_STATE, LAH_WHITESPACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{LAH_START_STATE, LAH_WHITESPACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{LAH_OPERAND_STATE, LAH_WHITESPACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{LAH_OPCODE_STATE, LAH_WHITESPACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{LAH_END_OPCODE_STATE, LAH_EOL_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{LAH_START_STATE, LAH_EOL_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{LAH_OPERAND_STATE, LAH_EOL_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{LAH_OPCODE_STATE, LAH_EOL_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		// Negative test path data
		{LAH_DONE_STATE, LAH_OPENBRACE_STATE_TRANSITION, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}}},
		{LAH_DONE_STATE, LAH_COMMA_STATE_TRANSITION,  {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 3, 0, 0, 0, 0, 0, 1, 0, 0}}},
		{LAH_DONE_STATE, LAH_CLOSEBRACE_STATE_TRANSITION, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0}}},
	};
	*test_data_size = (sizeof(static_global_test_data) / sizeof(Error_Reporting_Test_Data));
	*positive_path_test_count = 13;		// Count the lines of test_data above between the comments above.

	Error_Reporting_Test_Data* test_data = calloc(*test_data_size, sizeof(*test_data));

	for (size_t i = 0; i < *test_data_size; i++)
	{
		memcpy(&test_data[i], &static_global_test_data[i], sizeof(*test_data));
	}

	return test_data;
}

static bool unit_test_collect_error_reporting_data(size_t test_step)
{
	bool test_passed = true;
	UTL_Test_Log_Data* log_data = UTL_create_and_init_test_log_data(
		"unit_test_collect_error_reporting_data", test_passed, UTL_POSITIVE_PATH,
		test_step == 0);
	if (!log_data)
	{
		UTL_report_create_and_init_test_log_data_memory_failure(
			"unit_test_collect_error_reporting_data");
		return false;
	}

	size_t positivie_path_count = 0;
	size_t test_count = 0;
	Error_Reporting_Test_Data* test_data = init_error_report_data(
		&positivie_path_count, &test_count);
	if (!test_data)
	{
		UTL_report_create_and_init_test_log_data_memory_failure(
			"unit_test_collect_error_reporting_data");
		return false;
	}

	if (log_data->stand_alone)
	{
		UTL_va_log_fprintf("STARTING internal unit test for %s()\n\n", "collect_error_reporting_data");
	}

	test_passed = run_error_checking_unit_tests(log_data, positivie_path_count, test_data, test_count);

	if (log_data->stand_alone)
	{
		UTL_va_log_fprintf("\nENDING internal unit test for %s(\n\n", "collect_error_reporting_data");
	}

	free(test_data);
	free(log_data);

	return test_passed;
}

typedef bool (*state_machine_unit_test_function)(size_t test_step);

typedef struct unit_test_functions_and_args
{
	char* test_name;
	state_machine_unit_test_function func;
} State_Machine_Unit_Test_Functions;

/*
 * This function unit tests all the internal functions that support the
 * function get_state_transition_collect_parser_error_data(). If any of
 * these unit tests fail the unit test for lexical_analyzer() will not
 * execute.
 */
bool internal_tests_on_all_state_transitions(size_t test_step)
{
	bool all_tests_passed = true;

	State_Machine_Unit_Test_Functions unit_tests[] =
	{
#ifdef UNIT_TEST_DEBUG
		{"unit_test_syntax_states", unit_test_syntax_states},
#endif
		{"unit_test_get_alpha_input_transition_character_type",
			unit_test_get_alpha_input_transition_character_type},
		{"unit_test_get_transition_character_type",
			unit_test_get_transition_character_type},
		{"unit_test_collect_error_reporting_data",
			unit_test_collect_error_reporting_data},
	};
	size_t test_max = (sizeof(unit_tests) / sizeof(*unit_tests));

	for (size_t test_count = 0; test_count < test_max; test_count++)
	{
		bool test_passed = unit_tests[test_count].func(test_step);
		UTL_va_log_fprintf("\nSyntax Machine Internal Unit Test %zd: %s : %s\n\n",
			test_count + 1, unit_tests[test_count].test_name,
			(test_passed) ? "Passed" : "Failed");
		// if one test already failed we are good
		if (all_tests_passed)
		{
			all_tests_passed = test_passed;
		}
	}	

	return all_tests_passed;
}

static void report_syntax_errors(const unsigned necessary_items[])
{
	char* error_strings[LAH_SYNTAX_CHECK_ARRAY_SIZE];
	error_strings[LAH_OPENBRACE] = "Missing the opening brace.";
	error_strings[LAH_CLOSEBRACE] = "Missing the closing brace.";
	error_strings[LAH_COMMA] = "Missing comma(s)";
	error_strings[LAH_LEGALOPCODE] = "Missing or unknow opcode";
	error_strings[LAH_LEGALOPERAND] = "Missing operand or operand out of range";
	error_strings[LAH_ILLEGALOPCODE] = "Unknown Opcode.";
	error_strings[LAH_ILLEGALFIRSTCHAR] = "Illegal character in column 1 (are you missing the opening brace { )";
	error_strings[LAH_MULTIPLESTATEMENTSONELINE] = "Only one program step per line";
	error_strings[LAH_ILLEGALCHAR] = "Illegal Character";
	error_strings[LAH_MISSINGCOMMA] = "Missing comma(s)";

	for (size_t i = 0; i < LAH_SYNTAX_CHECK_ARRAY_SIZE; i++)
	{
		if (i >= LAH_ILLEGALOPCODE && necessary_items[i])
		{
			UTL_va_log_fprintf("\t%s\n", error_strings[i]);
		}
		else if (i < LAH_ILLEGALOPCODE && !necessary_items[i])
		{
			UTL_va_log_fprintf("\t%s\n", error_strings[i]);
		}
	}

}

static bool check_syntax_check_list_and_report_errors_as_parser_would(
	unsigned syntax_check_list[], LAH_Syntax_State state, unsigned char* text_line,
	size_t statement_number, Expected_Syntax_Errors* expected_errors,
	char *parser_generated_error)
{
	size_t error_count = 0;
	bool syntax_check_list_in_sync = true;

	for (size_t i = 0; i < LAH_SYNTAX_CHECK_ARRAY_SIZE; i++)
	{
		error_count += (!syntax_check_list[i] && i < LAH_ILLEGALOPCODE) ? 1 : ((i >= LAH_ILLEGALOPCODE && syntax_check_list[i]) ? 1 : 0);
		if (syntax_check_list[i] != expected_errors->syntax_check_list[i] && i != LAH_MULTIPLESTATEMENTSONELINE)
		{
			syntax_check_list_in_sync = false;
		}
	}

	if (error_count != expected_errors->error_count)
	{
		syntax_check_list_in_sync = false;
	}

	char* eol_p = strrchr((const char *)text_line, '\n');
	if (eol_p)
	{
		*eol_p = '\0';
	}

	if (state == LAH_ERROR_STATE || error_count)
	{
		UTL_va_log_fprintf("\n\nStatement %zu (%s) has the following syntax errors\n", statement_number + 1, text_line);
		if (parser_generated_error)
		{
			UTL_va_log_fprintf(parser_generated_error);
		}
		report_syntax_errors(syntax_check_list);
	}
	else
	{
		if (expected_errors->error_count)
		{
			UTL_va_log_fprintf("\n\nStatement %zu (%s)\n", statement_number + 1, text_line);
			UTL_va_log_fprintf("Expected syntax errors were:\n");
			report_syntax_errors(expected_errors->syntax_check_list);
		}
	}

	return syntax_check_list_in_sync;
}

static char* error_state(unsigned char* text_line, size_t statement_number, unsigned char* current_character)
{
	char* parser_generated_error;
	++statement_number;

	char* eol_p = strrchr((const char*)text_line, '\n');
	if (eol_p)
	{
		*eol_p = '\0';
	}

	char buffer[UTL_LOG_BUFFER_SIZE];
	snprintf(buffer, UTL_LOG_BUFFER_SIZE,
		"Syntax Error line %zu %s column %zu unexpected character '%c' : skipping rest of line.\n",
		statement_number, text_line, (size_t)(current_character - text_line),
		*current_character);
	parser_generated_error = SSF_strdup(buffer);

	return parser_generated_error;
}

/*
 * Provides debug data when a unit test fails.
 */
static void report_lexical_analyzer_test_failure(LAH_Syntax_State current_state, unsigned syntax_check_list[], Expected_Syntax_Errors* expected_errors)
{
	char out_buffer[UTL_LOG_BUFFER_SIZE];
	snprintf(out_buffer, UTL_LOG_BUFFER_SIZE, "\tcurrent_state = %s expected error count = %u ",
		state_name_for_printing(current_state), expected_errors->error_count);
	SSF_strcat(out_buffer, "expected Checklist Values {", sizeof(out_buffer));
	print_syntax_error_checklist(expected_errors->syntax_check_list, out_buffer, sizeof(out_buffer));
	SSF_strcat(out_buffer, "new checklist values {", sizeof(out_buffer));
	print_syntax_error_checklist(syntax_check_list, out_buffer, sizeof(out_buffer));
	SSF_strcat(out_buffer, "\n", sizeof(out_buffer));
	UTL_va_log_fprintf(out_buffer);
}

/*
 * This test parses a signle statement as the parser would. It directly calls
 * the lexical analiyzer for each character.
 */
static bool unit_test_final_lexical_parse_statement(unsigned char* text_line, size_t statement_number, UTL_Test_Log_Data* log_data, Expected_Syntax_Errors *expected_errors)
{
	bool test_passed = true;

	unsigned syntax_check_list[LAH_SYNTAX_CHECK_ARRAY_SIZE];
	memset(&syntax_check_list[0], 0, sizeof(syntax_check_list));
	LAH_Syntax_State current_state = LAH_START_STATE;
	unsigned char* opcode_start = NULL;
	unsigned char* opcode_end = NULL;
	unsigned char* operand_start = NULL;
	char* parser_generated_error = NULL;

	unsigned char* current_character = text_line;
	while (*current_character && current_state != LAH_ERROR_STATE)
	{
		LAH_Syntax_State new_state = lexical_analyzer(current_state, *current_character, syntax_check_list);
		if (new_state != current_state)
		{
			switch (new_state)
			{
				case LAH_ERROR_STATE:
				{
					parser_generated_error = error_state(text_line, statement_number, current_character);
				};
					break;

				case LAH_OPCODE_STATE:
					opcode_start = current_character;
					syntax_check_list[LAH_LEGALOPCODE]++;
					break;

				case LAH_END_OPCODE_STATE:
					opcode_end = current_character;
					break;

				case LAH_OPERAND_STATE:
					operand_start = current_character;
					syntax_check_list[LAH_LEGALOPERAND]++;
					if (!syntax_check_list[LAH_COMMA])
					{
						syntax_check_list[LAH_MISSINGCOMMA]++;
					}
					break;

				case LAH_END_OPERAND_STATE:
					opcode_end = current_character;
					break;

				default:
					break;
			}

			current_state = new_state;
		}

		current_character++;
	}

	bool syntax_check_list_in_sync = check_syntax_check_list_and_report_errors_as_parser_would(
		syntax_check_list, current_state, text_line, statement_number, expected_errors, parser_generated_error);

	if (!syntax_check_list_in_sync)
	{
		report_lexical_analyzer_test_failure(current_state, syntax_check_list, expected_errors);
		test_passed = false;
		log_data->status = false;
	}

	UTL_log_test_status_each_step2(log_data);
	free(parser_generated_error);

	return test_passed;
}

bool run_parse_program_loop(UTL_Test_Log_Data* log_data, Lexical_Analyzer_Test_Data* test_data)
{
	bool test_passed = true;

	unsigned char** test_program = test_data->test_program;
	Expected_Syntax_Errors* expected_errors = test_data->expected_errors;

	for (size_t test_count = 0; test_count < test_data->test_program_size; test_count++)
	{
		log_data->status = true;
		if (!unit_test_final_lexical_parse_statement(test_program[test_count], test_count,
			log_data, &expected_errors[test_count]))
		{
			test_passed = log_data->status;
		}
	}

	return test_passed;
}
/*
 * This final test imitates the parser and parses an entire program. There are
 * 2 programs, one without syntax errors and one with syntax errors. The positive
 * test path is the one without syntax errors and the negative path is the one
 * with syntax errors.
 */
bool unit_test_parse_statements_for_lexical_analysis(size_t test_step)
{
	bool test_passed = true;
	UTL_Test_Log_Data* log_data = UTL_create_and_init_test_log_data(
		"unit_test_parse_statements_for_lexical_analysis", test_passed, UTL_POSITIVE_PATH,
		test_step == 0);

	Lexical_Analyzer_Test_Data* positive_path_data = init_positive_path_data_for_lexical_analysis(log_data);
	if (!positive_path_data)
	{
		return false;
	}

	UTL_log_start_test_path(log_data);
	if (!run_parse_program_loop(log_data, positive_path_data))
	{
		test_passed = log_data->status;
	}
	UTL_log_end_test_path(log_data);


	Lexical_Analyzer_Test_Data* negative_path_data = init_negative_path_data_for_lexical_analysis(log_data);
	if (!negative_path_data)
	{
		return false;
	}

	log_data->path = UTL_NEGATIVE_PATH;
	UTL_log_start_test_path(log_data);
	char* explanation = "Only statements with syntax errors are printed"
		" Statement 1 and statement 8 do not contain syntax errors\n\n";
	UTL_va_log_fprintf(explanation);
	if (!run_parse_program_loop(log_data, negative_path_data))
	{
		test_passed = log_data->status;
	}
	UTL_log_end_test_path(log_data);

	deallocate_lexical_test_data(positive_path_data);
	deallocate_lexical_test_data(negative_path_data);
	free(log_data);

	return test_passed;
}

/* 
 * Unit test the public interface in lexical_analyzer.c. This function
 * assumes that internal_tests_on_all_state_transitions has been previously
 * called and that all component functions have been unit tested first. The 
 * public interface is tested in 2 ways, first with test data and then
 * parsing statements as the parser will.
 */
bool unit_test_lexical_analyzer(size_t test_step)
{
	bool test_passed = true;

	UTL_Test_Log_Data* log_data = UTL_create_and_init_test_log_data(
		"unit_test_lexical_analyzer", test_passed, UTL_POSITIVE_PATH,
		test_step == 0);
	if (!log_data)
	{
		UTL_report_create_and_init_test_log_data_memory_failure("unit_test_lexical_analyzer");
		return false;
	}

	if (log_data->stand_alone)
	{
		UTL_log_start_unit_test(log_data);
	}

	test_passed = unit_test_parse_statements_for_lexical_analysis(test_step);
	log_data->status = test_passed;

	if (log_data->stand_alone)
	{
		UTL_log_end_unit_test(log_data);
	}

	free(log_data);

	return test_passed;
}
#endif	// INTERNAL_SYNTAX_STATE_TESTS_C
