/*
 * internal_character_transition_unit_tests.c
 *
 * This file contains the lowest level of unit testing for the lexical analyzer.
 * It tests the lexical state transitions for particular characters.
 * The functions being tested are declared static in lexical_analyzer.c so
 * lexical_analyzer.c is included by this file to get access to these functions.
 */

#ifndef INTERNAL_CHARACTER_TRANSITION_UNIT_TEST_C
#define INTERNAL_CHARACTER_TRANSITION_UNIT_TEST_C
#define INCLUDED_IN_UNIT_TEST

#include <lexical_analyzer.c>
#include "lexical_analyzer_unit_test_utilities.h"
#include "internal_character_transition_unit_tests.h"

static void log_unit_test_get_transition_character_type_failure(
	Test_Log_Data* log_data, unsigned char candidate, LAH_Syntax_State current_state,
	LAH_State_Transition_Characters expected_type, LAH_State_Transition_Characters actual_type)
{
	// Force failures to be reported
	bool stand_alone = log_data->stand_alone;

	log_test_status_each_step2(log_data);

	char out_buffer[BUFSIZ];
	sprintf(out_buffer, "\tcurrent_state = %s input character = %c\n",
		state_name_for_printing(current_state), candidate);
	log_generic_message(out_buffer);

	sprintf(out_buffer, "\tExpected Transitiion %s Actual Transition %s\n\n",
		transition_names(expected_type), transition_names(actual_type));
	log_generic_message(out_buffer);

	log_data->stand_alone = stand_alone;
}

typedef enum test_character_case
{
	LOWER_CASE = 0,
	UPPER_CASE = 1
} TEST_CHARACTER_CASE;

static LAH_State_Transition_Characters get_expected_alpha_transition_character_type(
	unsigned char input, LAH_Syntax_State current_state)
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
		if (current_state == LAH_ENTER_OPERAND_STATE || current_state == LAH_OPERAND_STATE
			|| current_state == LAH_END_OPERAND_STATE)
		{
			return LAH_DIGIT_STATE_TRANSITION;
		}
		else
		{
			return LAH_ALPHA_STATE_TRANSITION;
		}
		break;

	default:
		return LAH_ALPHA_STATE_TRANSITION;
		break;
	}
}

typedef LAH_State_Transition_Characters(*STFfunct)(unsigned char input, LAH_Syntax_State current_state);
static bool core_alpha_character_transition_unit_test(Test_Log_Data* log_data, LAH_Syntax_State current_state, STFfunct transition_function)
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
			LAH_State_Transition_Characters expected_type = get_expected_alpha_transition_character_type(candidate_character, current_state);
			LAH_State_Transition_Characters actual_type = transition_function(candidate_character, current_state);
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
	LAH_Syntax_State current_state, unsigned char* input, LAH_State_Transition_Characters expected_transition[],
	size_t positive_path_count, char* local_func_name)
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
		LAH_State_Transition_Characters actual_transistion = get_transition_character_type(
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
bool unit_test_get_alpha_input_transition_character_type(unsigned test_step)
{
	bool test_passed = true;
	Test_Log_Data log_data;

	init_test_log_data(&log_data, "unit_test_get_alpha_input_transition_character_type",
		test_passed, "Positive", test_step == 0);

	if (log_data.stand_alone)
	{
		log_start_positive_path(log_data.function_name);
	}

	for (size_t state = (size_t)LAH_ENTER_OPCODE_STATE; state <= (size_t)LAH_END_OPERAND_STATE; state++)
	{
		test_passed = core_alpha_character_transition_unit_test(&log_data, state,
			get_alpha_input_transition_character_type);
	}

	if (log_data.stand_alone)
	{
		log_end_test_path(&log_data);
	}

	return test_passed;
}

static bool unit_test_whitespace_transition(Test_Log_Data* log_data, LAH_Syntax_State current_state)
{
	bool test_passed = true;
	unsigned char input[] = " \t\n\r\v\f";

	LAH_State_Transition_Characters expected_transition[] =
	{
		// Positive test path
		LAH_WHITESPACE_STATE_TRANSITION, LAH_WHITESPACE_STATE_TRANSITION, LAH_EOL_STATE_TRANSITION,
		// Test the negatvie path as well.
		LAH_EOL_STATE_TRANSITION, LAH_ILLEGAL_CHAR_TRANSITION, LAH_ILLEGAL_CHAR_TRANSITION
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

static void init_digit_test_data(unsigned char* input, LAH_State_Transition_Characters
	expected_transition[], size_t* positive_test_path, LAH_Syntax_State current_state)
{
	LAH_State_Transition_Characters* expected_ptr = expected_transition;
	if (current_state == LAH_ENTER_OPERAND_STATE || current_state == LAH_OPERAND_STATE || current_state == LAH_END_OPERAND_STATE)
	{
		for (; *input; input++, expected_ptr++)
		{
			*expected_ptr = LAH_DIGIT_STATE_TRANSITION;
		}
		*positive_test_path = strlen((const char*)input);
	}
	else
	{
		for (; *input; input++, expected_ptr++)
		{
			if (isdigit(*input))
			{
				*expected_ptr = LAH_DIGIT_STATE_TRANSITION;
				(*positive_test_path)++;
			}
			else
			{
				*expected_ptr = LAH_ALPHA_STATE_TRANSITION;		// to force failures use this instead *expected_ptr = DIGIT_STATE_TRANSITION;
			}
		}
	}
}

static bool unit_test_digit_transition(Test_Log_Data* log_data, LAH_Syntax_State current_state)
{
	bool test_passed = true;
	unsigned char* input = (unsigned char*)"0123456789ABCDEFXabcdefx";		// size is currently 24
#define MAX_INPUT_CHARACTERS	24
	LAH_State_Transition_Characters expected_transition[MAX_INPUT_CHARACTERS];
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
static bool unit_test_alpha_transition(Test_Log_Data* log_data, LAH_Syntax_State current_state)
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

static bool unit_test_punctuation_transition(Test_Log_Data* log_data, LAH_Syntax_State current_state)
{
	bool test_passed = true;
	unsigned char input[] = "{},+-/*=&";

	LAH_State_Transition_Characters expected_transition[] =
	{
		// Positive test path
		LAH_OPENBRACE_STATE_TRANSITION, LAH_CLOSEBRACE_STATE_TRANSITION, LAH_COMMA_STATE_TRANSITION,
		// Test the negatvie path as well.
		LAH_ILLEGAL_CHAR_TRANSITION, LAH_ILLEGAL_CHAR_TRANSITION, LAH_ILLEGAL_CHAR_TRANSITION,
		LAH_ILLEGAL_CHAR_TRANSITION, LAH_ILLEGAL_CHAR_TRANSITION, LAH_ILLEGAL_CHAR_TRANSITION
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

typedef bool (*character_transition_test_function)(Test_Log_Data* log_data, LAH_Syntax_State state);

bool unit_test_get_transition_character_type(size_t test_step)
{
	bool test_passed = true;
	char buffer[BUFSIZ];

	Test_Log_Data* log_data = create_and_init_test_log_data(
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

	character_transition_test_function test_function[] =
	{
		unit_test_punctuation_transition,
		unit_test_alpha_transition,
		unit_test_digit_transition,
		unit_test_whitespace_transition
	};
	for (size_t state = (size_t)LAH_START_STATE; state <= (size_t)LAH_ERROR_STATE; state++)
	{
		for (size_t unit_test_count = 0; unit_test_count < sizeof(test_function) /
			sizeof(*test_function); unit_test_count++)
		{
			if (!test_function[unit_test_count](log_data, (LAH_Syntax_State)state))
			{
				test_passed = log_data->status;
			}
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
#endif	// INTERNAL_CHARACTER_TRANSITION_UNIT_TEST_C
