/* 
 * internal_sytax_state_tests.c
 *
 * This file contains both internal syntax state machine unit tests, and unit tests
 * for the public interface of the lexitcal analizer these test functions test the
 * very basic functions that are the building blocks of the public interface, they are
 * declared static so these tests must be included in the syntax_state_machine.c file
 * rather than externally.
 */

#include "internal_sytax_state_tests.h"

typedef struct state_test_data
{
	Syntax_State current_state;
	Syntax_State expected_state;
	Syntax_Check_List_Items check_list_item_to_update;
	char input;
	unsigned check_list_item_prestate;
	unsigned expected_check_list_item_value;
	unsigned check_list_item_max_value;
} State_Test_Data;

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

static void log_unit_test_get_transition_character_type_failure(
	Test_Log_Data *log_data, unsigned char candidate, Syntax_State current_state,
	State_Transition_Characters expected_type, State_Transition_Characters actual_type)
{
	// Force failures to be reported
	bool stand_alone = log_data->stand_alone;

	log_test_status_each_step2(log_data);

	char out_buffer[BUFSIZ];
	sprintf(out_buffer, "\tcurrent_state = %s input character = %c\n",
		state_name_for_printing(current_state), candidate);
	log_generic_message(out_buffer);

	sprintf(out_buffer, "\tExpected Transitiion %s Actual Transition %s\n\n",
		transition_character[expected_type], transition_character[actual_type]);
	log_generic_message(out_buffer);

	log_data->stand_alone = stand_alone;
}

typedef enum test_character_case
{
	LOWER_CASE = 0,
	UPPER_CASE = 1
} TEST_CHARACTER_CASE;

static State_Transition_Characters get_expected_alpha_transition_character_type(
	unsigned char input, Syntax_State current_state)
{
	input = (unsigned char)toupper(input);

	switch (input)
	{
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'X':
			if (current_state == ENTER_OPERAND_STATE || current_state == OPERAND_STATE
				|| current_state == END_OPERAND_STATE)
			{
				return DIGIT_STATE_TRANSITION;
			}
			else
			{
				return ALPHA_STATE_TRANSITION;
			}
			break;

		default:
			return ALPHA_STATE_TRANSITION;
			break;
	}
}

typedef State_Transition_Characters(*STFfunct)(unsigned char input, Syntax_State current_state);
static bool core_alpha_character_transition_unit_test(Test_Log_Data* log_data, Syntax_State current_state, STFfunct transition_function)
{
	bool test_passed = true;
	char buffer[BUFSIZ];

	for (size_t alphabet = (size_t)LOWER_CASE; alphabet <= (size_t)UPPER_CASE; alphabet++)
	{
		if (log_data->stand_alone)
		{
			sprintf(buffer, "\tBegin Positive test path current_state = %s input character = %s\n\n",
				state_name_for_printing(current_state), (alphabet == LOWER_CASE) ? "Lower Case" : "Upper case");
			log_generic_message(buffer);
		}

		unsigned char fist_character_to_test = (alphabet == LOWER_CASE) ? 'a' : 'A';
		unsigned char last_character_to_test = (alphabet == LOWER_CASE) ? 'z' : 'Z';
		for (unsigned char candidate_character = fist_character_to_test; candidate_character <= last_character_to_test; candidate_character++)
		{
			log_data->status = true;
			State_Transition_Characters expected_type = get_expected_alpha_transition_character_type(candidate_character, current_state);
			State_Transition_Characters actual_type = transition_function(candidate_character, current_state);
			if (expected_type != actual_type)
			{
				log_data->status = false;
				test_passed = log_data->status;
				log_unit_test_get_transition_character_type_failure(log_data, candidate_character, current_state, expected_type, actual_type);
			}
			else
			{
				log_test_status_each_step2(log_data);
			}
		}

		if (log_data->stand_alone)
		{
			sprintf(buffer,
				"\n\tEnd Positive test path current_state = %s input character = %s\n\n",
				state_name_for_printing(current_state),
				(alphabet == LOWER_CASE) ? "Lower Case" : "Upper case");
			log_generic_message(buffer);
		}
	}

	return test_passed;
}

static bool core_non_alpha_character_transition_unit_test(Test_Log_Data* log_data,
	Syntax_State current_state, unsigned char* input, State_Transition_Characters expected_transition[],
	size_t positive_path_count, char *local_func_name)
{
	bool test_passed = true;
	char* keep_old_path = log_data->path;

	log_data->path = "Positive";
	size_t test_count = 0;
	for (unsigned char* test_input = input; *test_input; test_input++, test_count++)
	{
		if (positive_path_count == test_count)
		{
			log_end_positive_path(local_func_name);
			log_start_negative_path(local_func_name);
			log_data->path = "Negative";
		}

		log_data->status = true;
		State_Transition_Characters actual_transistion = get_transition_character_type(
			*test_input, current_state);
		log_data->status = actual_transistion == expected_transition[test_count];
		if (!log_data->status)
		{
			log_unit_test_get_transition_character_type_failure(log_data, *test_input,
				current_state, expected_transition[test_count], actual_transistion);
			test_passed = false;
		}
		else
		{
			log_test_status_each_step2(log_data);
		}
	}

	log_data->status = test_passed;
	log_data->path = keep_old_path;
	
	return test_passed;
}

/*
 * Tests limited number of states where alpha is important calls the lower level
 * function get_alpha_input_transition_character_type().
 */
static bool unit_test_get_alpha_input_transition_character_type(unsigned test_step)
{
	bool test_passed = true;
	Test_Log_Data log_data;

	init_test_log_data(&log_data, "unit_test_get_alpha_input_transition_character_type",
		test_passed, "Positive", test_step == 0);

	if (log_data.stand_alone)
	{
		log_start_positive_path(log_data.function_name);
	}

	for (size_t state = (size_t)ENTER_OPCODE_STATE; state <= (size_t)END_OPERAND_STATE; state++)
	{
		test_passed = core_alpha_character_transition_unit_test(&log_data, state,
			get_alpha_input_transition_character_type);
	}

	if (log_data.stand_alone)
	{
		log_end_positive_path(log_data.function_name);
	}

	return test_passed;
}

static bool unit_test_whitespace_transition(Test_Log_Data* log_data, Syntax_State current_state)
{
	bool test_passed = true;
	unsigned char input[] = " \t\n\r\v\f";

	State_Transition_Characters expected_transition[] =
	{
		// Positive test path
		WHITESPACE_STATE_TRANSITION, WHITESPACE_STATE_TRANSITION, EOL_STATE_TRANSITION,
		// Test the negatvie path as well.
		EOL_STATE_TRANSITION, ILLEGAL_CHAR_TRANSITION, ILLEGAL_CHAR_TRANSITION
	};
	size_t positive_path_count = 4;		// Change this if more positive path tests are added.

	char buffer[BUFSIZ];
	sprintf(buffer, "%s whitespace transition test", log_data->function_name);
	char* local_func_name = _strdup(buffer);

	log_start_positive_path(local_func_name);

	if (core_non_alpha_character_transition_unit_test(log_data, current_state,
		input, expected_transition, positive_path_count, local_func_name))
	{
		test_passed = log_data->status;
	}

	log_end_negative_path(local_func_name);
	free(local_func_name);

	log_data->status = test_passed;

	return test_passed;
}

static void init_digit_test_data(unsigned char *input, State_Transition_Characters
	expected_transition[], size_t *positive_test_path, Syntax_State current_state)
{
	State_Transition_Characters* expected_ptr = expected_transition;
	if (current_state == ENTER_OPERAND_STATE || current_state == OPERAND_STATE || current_state == END_OPERAND_STATE)
	{
		for ( ; *input; input++, expected_ptr++)
		{
			*expected_ptr = DIGIT_STATE_TRANSITION;
		}
		*positive_test_path = strlen((const char*)input);
	}
	else
	{
		for (; *input; input++, expected_ptr++)
		{
			if (isdigit(*input))
			{
				*expected_ptr = DIGIT_STATE_TRANSITION;
				(*positive_test_path)++;
			}
			else
			{
				*expected_ptr = ALPHA_STATE_TRANSITION;		// to force failures use this instead *expected_ptr = DIGIT_STATE_TRANSITION;
			}
		}
	}
}

static bool unit_test_digit_transition(Test_Log_Data* log_data, Syntax_State current_state)
{
	bool test_passed = true;
	unsigned char* input = (unsigned char *) "0123456789ABCDEFXabcdefx";		// size is currently 24
#define MAX_INPUT_CHARACTERS	24
	State_Transition_Characters expected_transition[MAX_INPUT_CHARACTERS];
	size_t positive_path_count;								// Change this if more positive path tests are added.
	init_digit_test_data(input, expected_transition, &positive_path_count, current_state);

	char* local_func_name = NULL;
	if (log_data->stand_alone)
	{
		char buffer[BUFSIZ];
		sprintf(buffer, "%s digit transition test", log_data->function_name);
		local_func_name = _strdup(buffer);
		log_start_positive_path(local_func_name);
	}

	if (core_non_alpha_character_transition_unit_test(log_data, current_state,
		input, expected_transition, positive_path_count, local_func_name))
	{
		test_passed = log_data->status;
	}

	if (log_data->stand_alone)
	{
		if (positive_path_count > 10)
		{
			log_end_positive_path(local_func_name);
		}
		else
		{
			log_end_negative_path(local_func_name);
		}
	}

#undef MAX_INPUT_CHARACTERS

	log_data->status = test_passed;
	return test_passed;
}

/*
 * test the state specified by the caller function. Calls the higher level function
 * get_transition_character_type().
 */
static bool unit_test_alpha_transition(Test_Log_Data* log_data, Syntax_State current_state)
{
	bool test_passed = true;
	char* local_func_name = NULL;

	if (log_data->stand_alone)
	{
		char buffer[BUFSIZ];
		sprintf(buffer, "%s alpha transition test", log_data->function_name);
		local_func_name = _strdup(buffer);
		log_start_positive_path(local_func_name);
	}

	test_passed = core_alpha_character_transition_unit_test(log_data, current_state,
		get_transition_character_type);

	if (log_data->stand_alone)
	{
		log_end_positive_path(local_func_name);
	}

	return test_passed;
}

static bool unit_test_punctuation_transition(Test_Log_Data* log_data, Syntax_State current_state)
{
	bool test_passed = true;
	unsigned char input[] = "{},+-/*=&";

	State_Transition_Characters expected_transition[] =
	{
		// Positive test path
		OPENBRACE_STATE_TRANSITION, CLOSEBRACE_STATE_TRANSITION, COMMA_STATE_TRANSITION,
		// Test the negatvie path as well.
		ILLEGAL_CHAR_TRANSITION, ILLEGAL_CHAR_TRANSITION, ILLEGAL_CHAR_TRANSITION,
		ILLEGAL_CHAR_TRANSITION, ILLEGAL_CHAR_TRANSITION, ILLEGAL_CHAR_TRANSITION
	};
	size_t positive_path_count = 3;		// Change this if more positive path tests are added.

	char buffer[BUFSIZ];
	sprintf(buffer, "%s punctuation transition test", log_data->function_name);
	char* local_func_name = _strdup(buffer);

	log_start_positive_path(local_func_name);

	if (core_non_alpha_character_transition_unit_test(log_data, current_state,
		input, expected_transition, positive_path_count, local_func_name))
	{
		test_passed = log_data->status;
	}

	log_end_negative_path(local_func_name);
	free(local_func_name);

	log_data->status = test_passed;

	return test_passed;
}

static bool unit_test_get_transition_character_type(size_t test_step)
{
//State_Transition_Characters get_transition_character_type(unsigned char input, Syntax_State current_state)
	bool test_passed = true;
	char buffer[BUFSIZ];
	Test_Log_Data *log_data = create_and_init_test_log_data(
		"unit_test_get_transition_character_type", test_passed, "Positive",
		test_step == 0);
	if (!log_data)
	{
		report_create_and_init_test_log_data_memory_failure(
			"unit_test_get_transition_character_type");
		return false;
	}

	if (log_data->stand_alone)
	{
		sprintf(buffer, "STARTING internal unit test for get_transition_character_type("
			"unsigned char input, Syntax_State current_state)");
		log_generic_message(buffer);
	}

	for (size_t state = (size_t)START_STATE; state <= (size_t)ERROR_STATE; state++)
	{
		if (!unit_test_punctuation_transition(log_data, (Syntax_State)state))
		{
			test_passed = log_data->status;
		}

		if (!unit_test_alpha_transition(log_data, state))
		{
			test_passed = log_data->status;
		}

		if (!unit_test_digit_transition(log_data, state))
		{
			test_passed = log_data->status;
		}

		if (!unit_test_whitespace_transition(log_data, state))
		{
			test_passed = log_data->status;
		}
	}

	if (log_data->stand_alone)
	{
		sprintf(buffer, "\nENDING internal unit test for get_transition_character_type("
			"unsigned char input, Syntax_State current_state)\n");
		log_generic_message(buffer);
	}

	free(log_data);

	return test_passed;
}

static void log_all_failure_data_for_unit_test_execute_transitions_all_states_special_characters(
	Test_Log_Data *log_data, State_Test_Data test_data[], unsigned syntax_check_list[],
	Syntax_State current_state, Syntax_State new_state, size_t test_count)
{
	log_test_status_each_step2(log_data);

	char out_buffer[BUFSIZ];
	sprintf(out_buffer, "\tcurrent_state = %s new_state = %s expected state = %s\n",
		state_name_for_printing(current_state),
		state_name_for_printing(new_state),
		state_name_for_printing(test_data[test_count].expected_state));
	log_generic_message(out_buffer);

	sprintf(out_buffer, "\tExpected Checklist Value %d new checklist value %d syntax_check_list[MULTIPLESTATEMENTSONELINE] %d\n\n",
		test_data[test_count].expected_check_list_item_value,
		syntax_check_list[test_data[test_count].check_list_item_to_update],
		syntax_check_list[MULTIPLESTATEMENTSONELINE]);
	log_generic_message(out_buffer);
}

static bool unit_test_execute_transitions_all_states_special_characters(
	Test_Log_Data log_data, size_t positive_path_test_count, State_Test_Data test_data[], size_t test_runs)
{
	bool test_passed = true;

	Syntax_State_Transition* next_states = get_or_create_next_states();
	if (!next_states)
	{
		fprintf(error_out_file, "In %s Memory allocation error in get_create_next_states()\n", log_data.function_name);
		return false;
	}

	log_start_positive_path(log_data.function_name);

	for (size_t test_count = 0; test_count < test_runs; test_count++)
	{
		log_data.status = true;
		if (test_count == positive_path_test_count)
		{
			log_end_positive_path(log_data.function_name);
			log_start_negative_path(log_data.function_name);
			log_data.path = "Negative";
		}

		unsigned syntax_check_list[SYNTAX_CHECK_COUNT];
		memset(&syntax_check_list[0], 0, sizeof(syntax_check_list));
		syntax_check_list[test_data[test_count].check_list_item_to_update] = test_data[test_count].check_list_item_prestate;

		Syntax_State current_state = test_data[test_count].current_state;
		Syntax_State new_state = test_function(current_state, next_states, syntax_check_list);

		if (new_state != test_data[test_count].expected_state
			|| syntax_check_list[test_data[test_count].check_list_item_to_update] != test_data[test_count].expected_check_list_item_value
			|| (test_data[test_count].expected_check_list_item_value > test_data[test_count].check_list_item_max_value && !syntax_check_list[MULTIPLESTATEMENTSONELINE])
			|| (test_data[test_count].expected_check_list_item_value <= test_data[test_count].check_list_item_max_value && syntax_check_list[MULTIPLESTATEMENTSONELINE]))
		{
			log_data.status = false;
			log_all_failure_data_for_unit_test_execute_transitions_all_states_special_characters(
				&log_data, test_data, syntax_check_list, current_state, new_state, test_count);
		}
		else
		{
			log_test_status_each_step2(&log_data);
		}

		if (!log_data.status && log_data.status != test_passed)
		{
			test_passed = log_data.status;
		}
	}

	log_end_negative_path(log_data.function_name);

	return test_passed;
}

static State_Test_Data static_global_test_data[] =
{
	// Start with positive test path data
	{START_STATE, ENTER_OPCODE_STATE, '{', OPENBRACE, 0, MAX_OPEN_BRACE, MAX_OPEN_BRACE},
	{END_STATEMENT_STATE, DONE_STATE, ',', COMMA, 1, 2, MAX_COMMA},
	{OPCODE_STATE, ENTER_OPERAND_STATE, ',', COMMA, 0, 1, MAX_COMMA},
	{END_OPCODE_STATE, ENTER_OPERAND_STATE, ',', COMMA, 0, 1, MAX_COMMA},
	{OPERAND_STATE, END_STATEMENT_STATE, '}', CLOSEBRACE, 0, MAX_CLOSE_BRACE, MAX_CLOSE_BRACE},
	// Negative test path data
	{START_STATE, ENTER_OPERAND_STATE, ',', COMMA, 0, 1, MAX_COMMA},
	{ENTER_OPCODE_STATE, ENTER_OPERAND_STATE, ',', COMMA, 0, 1, MAX_COMMA},
	{ENTER_OPERAND_STATE, DONE_STATE, ',', COMMA, 1, MAX_COMMA, MAX_COMMA},
	{OPERAND_STATE, DONE_STATE, ',', COMMA, 1, MAX_COMMA, MAX_COMMA},
	{END_OPERAND_STATE, DONE_STATE, ',', COMMA, 1, MAX_COMMA, MAX_COMMA},
	{ERROR_STATE, ERROR_STATE, ',', COMMA, 0, 1, MAX_COMMA},
	{END_STATEMENT_STATE, DONE_STATE, ',', COMMA, 2, 3, MAX_COMMA},	// too many commas
	{DONE_STATE, DONE_STATE, ',', COMMA, 2, 3, MAX_COMMA},	// too many commas
	{ENTER_OPCODE_STATE, ENTER_OPCODE_STATE, '{', OPENBRACE, 1, 2, MAX_OPEN_BRACE},
	{OPCODE_STATE, ERROR_STATE, '{', OPENBRACE, 1, 2, MAX_OPEN_BRACE},
	{END_OPCODE_STATE, ERROR_STATE, '{', OPENBRACE, 1, 2, MAX_OPEN_BRACE},
	{ENTER_OPERAND_STATE, ERROR_STATE, '{', OPENBRACE, 1, 2, MAX_OPEN_BRACE},
	{OPERAND_STATE, ERROR_STATE, '{', OPENBRACE, 1, 2, MAX_OPEN_BRACE},
	{END_OPERAND_STATE, ERROR_STATE, '{', OPENBRACE, 1, 2, MAX_OPEN_BRACE},
	{END_STATEMENT_STATE, ERROR_STATE, '{', OPENBRACE, 1, 2, MAX_OPEN_BRACE},
	{DONE_STATE, ERROR_STATE, '{', OPENBRACE, 1, 2, MAX_OPEN_BRACE},
	{ERROR_STATE, ERROR_STATE, '{', OPENBRACE, 1, 2, MAX_OPEN_BRACE},
	{START_STATE, ERROR_STATE, '}', CLOSEBRACE, 0, 1, MAX_CLOSE_BRACE},
	{ENTER_OPCODE_STATE, END_STATEMENT_STATE, '}', CLOSEBRACE, 0, 1, MAX_CLOSE_BRACE},
	{OPCODE_STATE, END_STATEMENT_STATE, '}', CLOSEBRACE, 0, 1, MAX_CLOSE_BRACE},
	{END_OPCODE_STATE, END_STATEMENT_STATE, '}', CLOSEBRACE, 0, 1, MAX_CLOSE_BRACE},
	{ENTER_OPERAND_STATE, END_STATEMENT_STATE, '}', CLOSEBRACE, 0, MAX_CLOSE_BRACE, MAX_CLOSE_BRACE},
	{END_OPERAND_STATE, END_STATEMENT_STATE, '}', CLOSEBRACE, 0, MAX_CLOSE_BRACE, MAX_CLOSE_BRACE},
	{END_STATEMENT_STATE, END_STATEMENT_STATE, '}', CLOSEBRACE, 1, 2, MAX_CLOSE_BRACE},
	{DONE_STATE, ERROR_STATE, '}', CLOSEBRACE, 1, 2, MAX_CLOSE_BRACE},
	{END_OPERAND_STATE, END_STATEMENT_STATE, '}', CLOSEBRACE, MAX_CLOSE_BRACE, MAX_CLOSE_BRACE + 1, MAX_CLOSE_BRACE}		// Positive, except for 2 close braces so negative
};
static size_t static_global_test_data_size = (sizeof(static_global_test_data) /sizeof(State_Test_Data) );
size_t static_global_positive_path_test_count = 5;		// Count the lines of test_data above between the comments.

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

	if (log_data->stand_alone)
	{
		sprintf(buffer, "STARTING internal unit test for collect_error_reporting_data("
			"Syntax_State current_state, State_Transition_Characters character_type, "
			"unsigned syntax_check_list[])\n\n");
		log_generic_message(buffer);
	}

	sprintf(buffer, "%s NOT IMPLEMENTED \n", log_data->function_name);
	log_generic_message(buffer);
	test_passed = false;

	if (log_data->stand_alone)
	{
		sprintf(buffer, "\nENDING internal unit test for collect_error_reporting_data("
			"Syntax_State current_state, State_Transition_Characters character_type, "
			"unsigned syntax_check_list[])\n\n");
		log_generic_message(buffer);
	}

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
			"unit_test_collect_error_reporting_data");
		return false;
	}

	if (log_data->stand_alone)
	{
		sprintf(buffer, "STARTING unit test for collect_error_reporting_data("
			"Syntax_State current_state, State_Transition_Characters character_type, "
			"unsigned syntax_check_list[])\n\n");
		log_generic_message(buffer);
	}

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
