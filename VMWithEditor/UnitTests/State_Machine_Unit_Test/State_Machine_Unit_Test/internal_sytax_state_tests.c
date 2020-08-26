/* 
 * internal_sytax_state_tests.c
 *
 * This file contains both internal syntax state machine unit tests, and unit tests
 * for the public interface of the lexitcal analyzer these test functions test the
 * very basic functions that are the building blocks of the public interface, they are
 * declared static so these tests must be included in the syntax_state_machine.c file
 * rather than externally.
 */

#include "internal_sytax_state_tests.h"
#include "lexical_analyzer_test_data.h"

static char *state_name_for_printing(Syntax_State state)
{
	char* state_names[SYNTAX_STATE_ARRAY_SIZE] =
	{
		"START_STATE",
		"ENTER_OPCODE_STATE",
		"OPCODE_STATE",
		"END_OPCODE_STATE",
		"ENTER_OPERAND_STATE",
		"OPERAND_STATE",
		"END_OPERAND_STATE",
		"END_STATEMENT_STATE",
		"DONE_STATE",
		"ERROR_STATE"
	};

	return state_names[(size_t)state];
}

static char* transition_character[TRANSITION_ARRAY_SIZE] =
{
	"Transition on {",
	"Transition on }",
	"Transition on ,",
	"Transition on Alpha",
	"Transition on Digit",
	"Transition on White Space",
	"Transition on EOL",
	"Transition on Illegal Character",
};

#ifdef UNIT_TEST_DEBUG
static bool unit_test_syntax_states(size_t test_step)
{
	bool test_passed = true;
	bool stand_alone = test_step == 0;

	Syntax_State_Transition* test_transitions = get_or_create_next_states();
	if (!test_transitions)
	{
		fprintf(error_out_file, "Memory allocation error in get_create_next_states()\n");
		return false;
	}

	for (size_t state = 0; state < SYNTAX_STATE_ARRAY_SIZE; state++)
	{
		char out_buffer[BUFSIZ];
		if (stand_alone)
		{
			sprintf(out_buffer, "current_state = %s\n", state_name_for_printing(
				test_transitions[state].current_state));
			log_generic_message(out_buffer);
		}

		if (stand_alone)
		{
			for (size_t character_index = 0; character_index < TRANSITION_ARRAY_SIZE;
				character_index++)
			{
				sprintf(out_buffer, "\ttransition character = %s\t\tnew state %s\n",
					transition_character[character_index],
					state_name_for_printing(
						test_transitions[state].transition_on_char_type[character_index]));
				log_generic_message(out_buffer);
			}
			log_generic_message("\n");
		}
	}

	return test_passed;
}
#endif

#include "internal_character_transition_unit_tests.c"

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
			sprintf(num_buff, "%d ,", syntax_checklist[i]);
			strcat(out_buffer, num_buff);
		}
		else
		{
			sprintf(num_buff, "%d} ", syntax_checklist[i]);
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
		{START_STATE, OPENBRACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{OPERAND_STATE, CLOSEBRACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{END_STATEMENT_STATE, COMMA_STATE_TRANSITION, {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 2, 0, 0, 0, 0, 0, 0, 0}}},
		{OPCODE_STATE, COMMA_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}}},
		{END_OPCODE_STATE, COMMA_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 1, 0, 0, 0, 0, 0, 0, 0}}},
		{END_OPCODE_STATE, WHITESPACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{START_STATE, WHITESPACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{OPERAND_STATE, WHITESPACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{OPCODE_STATE, WHITESPACE_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{END_OPCODE_STATE, EOL_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{START_STATE, EOL_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{OPERAND_STATE, EOL_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		{OPCODE_STATE, EOL_STATE_TRANSITION, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
		// Negative test path data
		{DONE_STATE, OPENBRACE_STATE_TRANSITION, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {2, 0, 0, 0, 0, 0, 0, 0, 1, 0}}},
		{DONE_STATE, COMMA_STATE_TRANSITION,  {0, 0, 2, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 0, 3, 0, 0, 0, 0, 0, 1, 0}}},
		{DONE_STATE, CLOSEBRACE_STATE_TRANSITION, {0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, {0, 2, 0, 0, 0, 0, 0, 0, 1, 0}}},
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
 * these unit tests fail the unit test for
 * get_state_transition_collect_parser_error_data() will not execute.
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
	size_t statement_number, Expected_Syntax_Errors* expected_errors)
{
	unsigned error_count = 0;
	bool syntax_check_list_in_sync = true;

	for (size_t i = 0; i < SYNTAX_CHECK_COUNT; i++)
	{
		error_count += (!syntax_check_list[i] && i < ILLEGALOPCODE) ? 1 : ((i >= ILLEGALOPCODE && syntax_check_list[i]) ? 1 : 0);
		if (syntax_check_list[i] != expected_errors->syntax_check_list[i])
		{
			syntax_check_list_in_sync = false;
		}
	}

	if (error_count != expected_errors->error_count)
	{
		syntax_check_list_in_sync = false;
	}

	char buffer[BUFSIZ];
	if (state == ERROR_STATE || error_count)
	{
		sprintf(buffer, "\n\nStatement %d (%s) has the following syntax errors\n", statement_number + 1, text_line);
		log_generic_message(buffer);
		report_syntax_errors(syntax_check_list);
	}
	else
	{
		sprintf(buffer, "\n\nStatement %d (%s)\n", statement_number + 1, text_line);
		log_generic_message(buffer);
		if (expected_errors->error_count)
		{
			sprintf(buffer, "Expected syntax errors were:\n");
			log_generic_message(buffer);
			report_syntax_errors(expected_errors->syntax_check_list);
		}
	}

	return syntax_check_list_in_sync;
}

static bool unit_test_final_lexical_check_line_syntax(unsigned char* text_line, size_t statement_number, Test_Log_Data* log_data, Expected_Syntax_Errors *expected_errors)
{
	bool test_passed = true;

	unsigned syntax_check_list[SYNTAX_CHECK_COUNT];
	memset(&syntax_check_list[0], 0, sizeof(syntax_check_list));
	Syntax_State current_state = START_STATE;
	unsigned char* opcode_start = NULL;
	unsigned char* opcode_end = NULL;
	unsigned char* operand_start = NULL;
	unsigned char* operand_end = NULL;

	unsigned char* current_character = text_line;
	while (*current_character && current_state != DONE_STATE && current_state != ERROR_STATE)
	{
		Syntax_State new_state = lexical_analyzer(current_state, *current_character, syntax_check_list);
		if (new_state != current_state)
		{
			switch (new_state)
			{
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
		syntax_check_list, current_state, text_line, statement_number, expected_errors);

	if (!syntax_check_list_in_sync)
	{
 		test_passed = false;
		log_data->status = false;
	}
	log_test_status_each_step2(log_data);

	return test_passed;
}

bool run_syntax_check_loop(Test_Log_Data* log_data, Lexical_Analyzer_Test_Data* test_data)
{
	bool test_passed = true;

	unsigned char** test_program = test_data->test_program;
	Expected_Syntax_Errors* expected_errors = test_data->expected_errors;

	for (size_t test_count = 0; test_count < test_data->test_program_size; test_count++)
	{
		log_data->status = true;
		if (!unit_test_final_lexical_check_line_syntax(test_program[test_count], test_count, log_data, &expected_errors[test_count]))
		{
			test_passed = log_data->status;
		}
	}

	return test_passed;
}
/*
 * This final test imitates the parser and parses an entire program.
 */
bool unit_test_parse_statements_for_lexical_analysis(unsigned test_step)
{
	bool test_passed = true;
	Test_Log_Data* log_data = create_and_init_test_log_data(
		"final_lexical_analysis_test", test_passed, "Positive",
		test_step == 0);

	Lexical_Analyzer_Test_Data* positive_path_data = init_positive_path_data_for_lexical_analysis(log_data);
	if (!positive_path_data)
	{
		return false;
	}

	log_start_test_path(log_data);
	if (!run_syntax_check_loop(log_data, positive_path_data))
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
	if (!run_syntax_check_loop(log_data, negative_path_data))
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
 * Unit test the public interface in syntax_state_machine.c. This function
 * assumes that internal_tests_on_all_state_transitions has been previously
 * called and that all component functions have been unit tested first. The 
 * public interface is tested in 2 ways, first with test data and then
 * parsing statements as the parser will.
 */
bool unit_test_get_state_transition_collect_parser_error_data(unsigned test_step)
{
	//Syntax_State get_state_transition_collect_parser_error_data(Syntax_State current_state, unsigned char* input, unsigned syntax_check_list[])
	bool test_passed = true;
	char buffer[BUFSIZ];
	Test_Log_Data* log_data = create_and_init_test_log_data(
		"unit_test_get_state_transition_collect_parser_error_data", test_passed, "Positive",
		test_step == 0);
	if (!log_data)
	{
		report_create_and_init_test_log_data_memory_failure(
			"unit_test_get_state_transition_collect_parser_error_data");
		return false;
	}

	if (log_data->stand_alone)
	{
		sprintf(buffer, "STARTING unit test for collect_error_reporting_data("
			"Syntax_State current_state, State_Transition_Characters character_type, "
			"unsigned syntax_check_list[])\n\n");
		log_generic_message(buffer);
	}

	test_passed = unit_test_parse_statements_for_lexical_analysis(test_step);

	sprintf(buffer, "%s NOT IMPLEMENTED \n", log_data->function_name);
	log_generic_message(buffer);
	test_passed = false;

	if (log_data->stand_alone)
	{
		sprintf(buffer, "\nENDING unit test for collect_error_reporting_data("
			"Syntax_State current_state, State_Transition_Characters character_type, "
			"unsigned syntax_check_list[])\n\n");
		log_generic_message(buffer);
	}

	free(log_data);

	return test_passed;
}
