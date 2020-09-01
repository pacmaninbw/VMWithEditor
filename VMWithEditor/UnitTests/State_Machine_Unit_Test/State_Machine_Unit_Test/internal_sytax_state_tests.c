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

#include "lexical_analyzer.h"
#include "lexical_analyzer.c"

#include "internal_sytax_state_tests.h"
#include "internal_character_transition_unit_tests.h"
#include "lexical_analyzer_test_data.h"
#include "lexical_analyzer_unit_test_utilities.h"

typedef struct state_test_data
{
	Syntax_State current_state;
	State_Transition_Characters input_character_state;
	unsigned syntax_items_checklist[SYNTAX_CHECK_COUNT];
	Expected_Syntax_Errors expected_data;
} Error_Reporting_Test_Data;

static void print_syntax_error_checklist(unsigned syntax_checklist[], char *out_buffer)
{
	for (size_t i = 0; i < SYNTAX_CHECK_COUNT; i++)
	{
		char num_buff[8];
		if (i < SYNTAX_CHECK_COUNT - 1)
		{
			sprintf(num_buff, "%u, ", syntax_checklist[i]);
			strcat(out_buffer, num_buff);
		}
		else
		{
			sprintf(num_buff, "%u} ", syntax_checklist[i]);
			strcat(out_buffer, num_buff);
		}
	}
}
static void log_all_failure_data_for_unit_test_collect_error_reporting_data(
	Test_Log_Data* log_data, Error_Reporting_Test_Data test_data, unsigned syntax_check_list[])
{
	log_test_status_each_step2(log_data);

	char out_buffer[BUFSIZ];
	sprintf(out_buffer, "\tcurrent_state = %s ", state_name_for_printing(test_data.current_state));
	strcat(out_buffer, "expected Checklist Values {");
	print_syntax_error_checklist(test_data.expected_data.syntax_check_list, out_buffer);
	strcat(out_buffer, "new checklist value {");
	print_syntax_error_checklist(syntax_check_list, out_buffer);
	strcat(out_buffer, "\n");
	log_generic_message(out_buffer);
}

static bool errors_in_sync(unsigned syntax_check_list[], Expected_Syntax_Errors expected_errors)
{
	bool syntax_check_list_in_sync = true;

	for (size_t i = 0; i < SYNTAX_CHECK_COUNT; i++)
	{
		if (syntax_check_list[i] != expected_errors.syntax_check_list[i])
		{
			syntax_check_list_in_sync = false;
		}
	}

	return syntax_check_list_in_sync;
}

static bool run_error_checking_unit_tests(
	Test_Log_Data *log_data, size_t positive_path_test_count,
	Error_Reporting_Test_Data test_data[], size_t test_runs)
{
	bool test_passed = true;

	log_start_test_path(log_data);

	for (size_t test_count = 0; test_count < test_runs; test_count++)
	{
		log_data->status = true;
		if (test_count == positive_path_test_count)
		{
			log_end_test_path(log_data);
			log_data->path = "Negative";
			log_start_test_path(log_data);
		}

		unsigned syntax_check_list[SYNTAX_CHECK_COUNT];
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
			log_test_status_each_step2(log_data);
		}

		if (!log_data->status && test_passed)
		{
			test_passed = log_data->status;
		}
	}

	log_end_test_path(log_data);

	return test_passed;
}

static Error_Reporting_Test_Data* init_error_report_data(size_t *positive_path_test_count, size_t *test_data_size)
{
	Error_Reporting_Test_Data static_global_test_data[] =
	{
		// Start with positive test path data
		{START_STATE, OPENBRACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{OPERAND_STATE, CLOSEBRACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{END_STATEMENT_STATE, COMMA_STATE_TRANSITION, {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{OPCODE_STATE, COMMA_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{END_OPCODE_STATE, COMMA_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{END_OPCODE_STATE, WHITESPACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{START_STATE, WHITESPACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{OPERAND_STATE, WHITESPACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{OPCODE_STATE, WHITESPACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{END_OPCODE_STATE, EOL_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{START_STATE, EOL_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{OPERAND_STATE, EOL_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{OPCODE_STATE, EOL_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		// Negative test path data
		{DONE_STATE, OPENBRACE_STATE_TRANSITION, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}}},
		{DONE_STATE, COMMA_STATE_TRANSITION,  {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 3, 0, 0, 0, 0, 0, 1, 0, 0}}},
		{DONE_STATE, CLOSEBRACE_STATE_TRANSITION, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0}}},
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

static bool unit_test_collect_error_reporting_data(unsigned test_step)
{
	bool test_passed = true;
	char buffer[BUFSIZ];
	Test_Log_Data* log_data = create_and_init_test_log_data(
		"unit_test_collect_error_reporting_data", test_passed, "Positive",
		test_step == 0);
	if (!log_data)
	{
		report_create_and_init_test_log_data_memory_failure(
			"unit_test_collect_error_reporting_data");
		return false;
	}

	size_t positivie_path_count = 0;
	size_t test_count = 0;
	Error_Reporting_Test_Data* test_data = init_error_report_data(&positivie_path_count, &test_count);
	if (!test_data)
	{
		fprintf(error_out_file, "Memory allocation of test_data failed in %s",
			log_data->function_name);
		return false;
	}

	if (log_data->stand_alone)
	{
		sprintf(buffer, "STARTING internal unit test for %s()\n\n", "collect_error_reporting_data");
		log_generic_message(buffer);
	}

	test_passed = run_error_checking_unit_tests(log_data, positivie_path_count, test_data, test_count);

	if (log_data->stand_alone)
	{
		sprintf(buffer, "\nENDING internal unit test for %s(\n\n", "collect_error_reporting_data");
		log_generic_message(buffer);
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
bool internal_tests_on_all_state_transitions(unsigned test_step)
{
	bool all_tests_passed = true;
	char buffer[BUFSIZ];

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
		sprintf(buffer, "\nSyntax Machine Internal Unit Test %zd: %s : %s\n\n",
			test_count + 1, unit_tests[test_count].test_name,
			(test_passed) ? "Passed" : "Failed");
		log_generic_message(buffer);
		// if one test already failed we are good
		if (all_tests_passed)
		{
			all_tests_passed = test_passed;
		}
	}	

	return all_tests_passed;
}

static void report_syntax_errors(unsigned necessary_items[])
{
	char* error_strings[SYNTAX_CHECK_COUNT];
	error_strings[OPENBRACE] = "Missing the opening brace.";
	error_strings[CLOSEBRACE] = "Missing the closing brace.";
	error_strings[COMMA] = "Missing comma(s)";
	error_strings[LEGALOPCODE] = "Missing or unknow opcode";
	error_strings[LEGALOPERAND] = "Missing operand or operand out of range";
	error_strings[ILLEGALOPCODE] = "Unknown Opcode.";
	error_strings[ILLEGALFIRSTCHAR] = "Illegal character in column 1 (are you missing the opening brace { )";
	error_strings[MULTIPLESTATEMENTSONELINE] = "Only one program step per line";
	error_strings[ILLEGALCHAR] = "Illegal Character";
	error_strings[MISSINGCOMMA] = "Missing comma(s)";

	for (size_t i = 0; i < SYNTAX_CHECK_COUNT; i++)
	{
		char buffer[BUFSIZ];
		if (i >= ILLEGALOPCODE && necessary_items[i])
		{
			sprintf(buffer, "\t%s\n", error_strings[i]);
			log_generic_message(buffer);
		}
		else if (i < ILLEGALOPCODE && !necessary_items[i])
		{
			sprintf(buffer, "\t%s\n", error_strings[i]);
			log_generic_message(buffer);
		}
	}

}

static bool check_syntax_check_list_and_report_errors_as_parser_would(
	unsigned syntax_check_list[], Syntax_State state, unsigned char* text_line,
	size_t statement_number, Expected_Syntax_Errors* expected_errors,
	char *parser_generated_error)
{
	unsigned error_count = 0;
	bool syntax_check_list_in_sync = true;

	for (size_t i = 0; i < SYNTAX_CHECK_COUNT; i++)
	{
		error_count += (!syntax_check_list[i] && i < ILLEGALOPCODE) ? 1 : ((i >= ILLEGALOPCODE && syntax_check_list[i]) ? 1 : 0);
		if (syntax_check_list[i] != expected_errors->syntax_check_list[i] && i != MULTIPLESTATEMENTSONELINE)
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
	char buffer[BUFSIZ];
	if (state == ERROR_STATE || error_count)
	{
		sprintf(buffer, "\n\nStatement %zu (%s) has the following syntax errors\n", statement_number + 1, text_line);
		log_generic_message(buffer);
		if (parser_generated_error)
		{
			log_generic_message(parser_generated_error);
		}
		report_syntax_errors(syntax_check_list);
	}
	else
	{
		if (expected_errors->error_count)
		{
			sprintf(buffer, "\n\nStatement %zu (%s)\n", statement_number + 1, text_line);
			log_generic_message(buffer);
			sprintf(buffer, "Expected syntax errors were:\n");
			log_generic_message(buffer);
			report_syntax_errors(expected_errors->syntax_check_list);
		}
	}

	return syntax_check_list_in_sync;
}

static char* error_state(unsigned char* text_line, size_t statement_number, unsigned char* current_character)
{
	char* parser_generated_error;

	char buffer[BUFSIZ];
	char* eol_p = strrchr((const char*)text_line, '\n');
	if (eol_p)
	{
		*eol_p = '\0';
	}
	sprintf(buffer,
		"Syntax Error line %zd %s column %zu unexpected character '%c' : skipping rest of line.\n",
		statement_number + 1, text_line, (int)(current_character - text_line),
		*current_character);
	parser_generated_error = strdup(buffer);

	return parser_generated_error;
}

/*
 * Provides debug data when a unit test fails.
 */
static void report_lexical_analyzer_test_failure(Syntax_State current_state, unsigned syntax_check_list[], Expected_Syntax_Errors* expected_errors)
{
	char out_buffer[BUFSIZ];
	sprintf(out_buffer, "\tcurrent_state = %s expected error count = %u ",
		state_name_for_printing(current_state), expected_errors->error_count);
	strcat(out_buffer, "expected Checklist Values {");
	print_syntax_error_checklist(expected_errors->syntax_check_list, out_buffer);
	strcat(out_buffer, "new checklist values {");
	print_syntax_error_checklist(syntax_check_list, out_buffer);
	strcat(out_buffer, "\n");
	log_generic_message(out_buffer);
}

/*
 * This test parses a signle statement as the parser would. It directly calls
 * the lexical analiyzer for each character.
 */
static bool unit_test_final_lexical_parse_statement(unsigned char* text_line, size_t statement_number, Test_Log_Data* log_data, Expected_Syntax_Errors *expected_errors)
{
	bool test_passed = true;

	unsigned syntax_check_list[SYNTAX_CHECK_COUNT];
	memset(&syntax_check_list[0], 0, sizeof(syntax_check_list));
	Syntax_State current_state = START_STATE;
	unsigned char* opcode_start = NULL;
	unsigned char* opcode_end = NULL;
	unsigned char* operand_start = NULL;
	char* parser_generated_error = NULL;

	unsigned char* current_character = text_line;
	while (*current_character && current_state != ERROR_STATE)
	{
		Syntax_State new_state = lexical_analyzer(current_state, *current_character, syntax_check_list);
		if (new_state != current_state)
		{
			switch (new_state)
			{
				case ERROR_STATE:
				{
					parser_generated_error = error_state(text_line, statement_number, current_character);
				};
					break;

				case OPCODE_STATE:
					opcode_start = current_character;
					syntax_check_list[LEGALOPCODE]++;
					break;

				case END_OPCODE_STATE:
					opcode_end = current_character;
					break;

				case OPERAND_STATE:
					operand_start = current_character;
					syntax_check_list[LEGALOPERAND]++;
					if (!syntax_check_list[COMMA])
					{
						syntax_check_list[MISSINGCOMMA]++;
					}
					break;

				case END_OPERAND_STATE:
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

	log_test_status_each_step2(log_data);
	free(parser_generated_error);

	return test_passed;
}

bool run_parse_program_loop(Test_Log_Data* log_data, Lexical_Analyzer_Test_Data* test_data)
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
bool unit_test_parse_statements_for_lexical_analysis(unsigned test_step)
{
	bool test_passed = true;
	Test_Log_Data* log_data = create_and_init_test_log_data(
		"unit_test_parse_statements_for_lexical_analysis", test_passed, "Positive",
		test_step == 0);

	Lexical_Analyzer_Test_Data* positive_path_data = init_positive_path_data_for_lexical_analysis(log_data);
	if (!positive_path_data)
	{
		return false;
	}

	log_start_test_path(log_data);
	if (!run_parse_program_loop(log_data, positive_path_data))
	{
		test_passed = log_data->status;
	}
	log_end_test_path(log_data);


	Lexical_Analyzer_Test_Data* negative_path_data = init_negative_path_data_for_lexical_analysis(log_data);
	if (!negative_path_data)
	{
		return false;
	}

	log_data->path = "Negative";
	log_start_test_path(log_data);
	char* explanation = "Only statements with syntax errors are printed"
		" Statement 1 and statement 8 do not contain syntax errors\n\n";
	log_generic_message(explanation);
	if (!run_parse_program_loop(log_data, negative_path_data))
	{
		test_passed = log_data->status;
	}
	log_end_test_path(log_data);

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
bool unit_test_lexical_analyzer(unsigned test_step)
{
	//Syntax_State get_state_transition_collect_parser_error_data(Syntax_State current_state, unsigned char* input, unsigned syntax_check_list[])
	bool test_passed = true;
	char buffer[BUFSIZ];

	Test_Log_Data* log_data = create_and_init_test_log_data(
		"unit_test_lexical_analyzer", test_passed, "Positive",
		test_step == 0);
	if (!log_data)
	{
		report_create_and_init_test_log_data_memory_failure("unit_test_lexical_analyzer");
		return false;
	}

	if (log_data->stand_alone)
	{
		sprintf(buffer, "STARTING unit test for %s\n\n", log_data->function_name);
		log_generic_message(buffer);
	}

	test_passed = unit_test_parse_statements_for_lexical_analysis(test_step);

	if (log_data->stand_alone)
	{
		sprintf(buffer, "\nENDING unit test for %s\n\n", log_data->function_name);
		log_generic_message(buffer);
	}

	free(log_data);

	return test_passed;
}
#endif	// INTERNAL_SYNTAX_STATE_TESTS_C
