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

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "vmwitheditor.h"
#include "UTL_unit_test_logging.h"
#include "ERH_error_reporting.h"
#include "LAH_lexical_analyzer.h"
#include "lexical_analyzer.c"
#include "lexical_analyzer_unit_test_utilities.h"
#include "internal_character_transition_unit_tests.h"

static void log_unit_test_get_transition_character_type_failure(
	UTL_Test_Log_Data* log_data, Const_U_Char candidate, const LAH_Syntax_State current_state,
	const LAH_State_Transition_Characters expected_type, const LAH_State_Transition_Characters actual_type)
{
	// Force failures to be reported
	bool stand_alone = log_data->stand_alone;
	log_data->stand_alone = false;

	UTL_log_test_status_each_step(log_data);

	UTL_va_log_fprintf("\tcurrent_state = %s input character = %c\n",
		state_name_for_printing(current_state), candidate);

	UTL_va_log_fprintf("\tExpected Transitiion %s Actual Transition %s\n\n",
		transition_names(expected_type), transition_names(actual_type));

	log_data->stand_alone = stand_alone;
}

typedef enum test_character_case
{
	LOWER_CASE = 0,
	UPPER_CASE = 1
} TEST_CHARACTER_CASE;

static LAH_State_Transition_Characters get_expected_alpha_transition_character_type(
	Const_U_Char input, const LAH_Syntax_State current_state)
{
	LAH_State_Transition_Characters expected_transition = LAH_ALPHA_STATE_TRANSITION;
	if (isxdigit(input) || (input == 'x' || input == 'X'))
	{
		if (current_state == LAH_ENTER_OPERAND_STATE || current_state == LAH_OPERAND_STATE
			|| current_state == LAH_END_OPERAND_STATE)
		{
			expected_transition = LAH_DIGIT_STATE_TRANSITION;
		}
	}

	return expected_transition;
}

typedef LAH_State_Transition_Characters(*STFfunct)(Const_U_Char input, const LAH_Syntax_State current_state);
static bool core_alpha_character_transition_unit_test(UTL_Test_Log_Data* log_data, const LAH_Syntax_State current_state, STFfunct transition_function)
{
	bool test_passed = true;

	for (size_t alphabet = (size_t)LOWER_CASE; alphabet <= (size_t)UPPER_CASE; alphabet++)
	{
		UTL_va_test_log_fprintf(log_data, false,
			"\tBegin Positive test path current_state = %s input character = %s\n\n",
			state_name_for_printing(current_state),
			(alphabet == LOWER_CASE) ? "Lower Case" : "Upper case");

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
				UTL_log_test_status_each_step(log_data);
			}
		}

		UTL_va_test_log_fprintf(log_data, false,
			"\n\tEnd Positive test path current_state = %s input character = %s\n\n",
			state_name_for_printing(current_state),
			(alphabet == LOWER_CASE) ? "Lower Case" : "Upper case");
	}

	return test_passed;
}

static bool core_non_alpha_character_transition_unit_test(UTL_Test_Log_Data* log_data,
	const LAH_Syntax_State current_state, Const_U_Char* input,
	LAH_State_Transition_Characters expected_transition[],
	size_t positive_path_count)
{
	bool test_passed = true;
	log_data->path = UTL_POSITIVE_PATH;

	UTL_log_start_test_path(log_data);

	size_t test_count = 0;
	for (Const_U_Char* test_input = input; *test_input; test_input++, test_count++)
	{
		if (positive_path_count == test_count)
		{
			UTL_log_end_test_path(log_data);
			log_data->path = UTL_NEGATIVE_PATH;
			UTL_log_start_test_path(log_data);
		}

		log_data->status = true;
		LAH_State_Transition_Characters actual_transistion =
			get_transition_character_type( *test_input, current_state);
		log_data->status = actual_transistion == expected_transition[test_count];
		if (!log_data->status)
		{
			log_unit_test_get_transition_character_type_failure(log_data, *test_input,
				current_state, expected_transition[test_count], actual_transistion);
			test_passed = false;
		}
		else
		{
			UTL_log_test_status_each_step(log_data);
		}
	}

	UTL_log_end_test_path(log_data);

	log_data->status = test_passed;

	return test_passed;
}

/*
 * Tests limited number of states where alpha is important calls the lower level
 * function get_alpha_input_transition_character_type().
 */
bool unit_test_get_alpha_input_transition_character_type(const size_t test_step)
{
	bool test_passed = true;
	UTL_Test_Log_Data *log_data = UTL_create_and_init_test_log_data(
		"unit_test_get_alpha_input_transition_character_type",
		test_passed, UTL_POSITIVE_PATH, test_step == 0, true);

	UTL_log_start_test_path(log_data);

	for (size_t state = (size_t)LAH_ENTER_OPCODE_STATE; state <= (size_t)LAH_END_OPERAND_STATE; state++)
	{
		test_passed = core_alpha_character_transition_unit_test(log_data, state,
			get_alpha_input_transition_character_type);
	}

	UTL_log_end_test_path(log_data);
	free(log_data);

	return test_passed;
}

static bool temp_function_name(UTL_Test_Log_Data* log_data, char* appendage)
{
	char buffer[UTL_LOG_BUFFER_SIZE] = {0};
	SSF_strcat(buffer, (char *)log_data->function_name, sizeof(buffer));
	SSF_strcat(buffer, " ", sizeof(buffer));
	SSF_strcat(buffer, appendage, sizeof(buffer));
	const char* temp_name = SSF_strdup(buffer);
	if (!temp_name)
	{
		SSF_report_strdup_failure(buffer, "temp_name");
		return false;
	}
	log_data->function_name = temp_name;

	return true;
}

static bool unit_test_whitespace_transition(UTL_Test_Log_Data* log_data, const LAH_Syntax_State current_state)
{
	bool test_passed = true;
	Const_U_Char input[] = " \t\n\r\v\f";

	LAH_State_Transition_Characters expected_transition[] =
	{
		// Positive test path
		LAH_WHITESPACE_STATE_TRANSITION, LAH_WHITESPACE_STATE_TRANSITION, LAH_EOL_STATE_TRANSITION,
		// Test the negatvie path as well.
		LAH_EOL_STATE_TRANSITION, LAH_ILLEGAL_CHAR_TRANSITION, LAH_ILLEGAL_CHAR_TRANSITION
	};
	size_t positive_path_count = 4;		// Change this if more positive path tests are added.

	const char* real_function_name = log_data->function_name;
	if (!temp_function_name(log_data, "whitespace transition test"))
	{
		return false;
	}

	if (core_non_alpha_character_transition_unit_test(log_data, current_state,
		input, expected_transition, positive_path_count))
	{
		test_passed = log_data->status;
	}

	free((void *)log_data->function_name);

	log_data->function_name = real_function_name;
	log_data->status = test_passed;

	return test_passed;
}

static void init_digit_test_data(Const_U_Char* input, LAH_State_Transition_Characters
	expected_transition[], size_t* positive_test_path, const LAH_Syntax_State current_state)
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

static bool unit_test_digit_transition(UTL_Test_Log_Data* log_data, const LAH_Syntax_State current_state)
{
	bool test_passed = true;
	Const_U_Char* input = (unsigned char*)"0123456789ABCDEFXabcdefx";		// size is currently 24
#define MAX_INPUT_CHARACTERS	24
	LAH_State_Transition_Characters expected_transition[MAX_INPUT_CHARACTERS];
	size_t positive_path_count;								// Change this if more positive path tests are added.
	init_digit_test_data(input, expected_transition, &positive_path_count, current_state);

	const char* real_function_name = log_data->function_name;
	if (!temp_function_name(log_data, "digit transition test"))
	{
		return false;
	}

	if (core_non_alpha_character_transition_unit_test(log_data, current_state,
		input, expected_transition, positive_path_count))
	{
		test_passed = log_data->status;
	}

	free((void *)log_data->function_name);
#undef MAX_INPUT_CHARACTERS
	
	log_data->function_name = real_function_name;
	log_data->status = test_passed;
	return test_passed;
}

/*
 * test the state specified by the caller function. Calls the higher level function
 * get_transition_character_type().
 */
static bool unit_test_alpha_transition(UTL_Test_Log_Data* log_data, const LAH_Syntax_State current_state)
{
	bool test_passed = true;
	const char* real_function_name = log_data->function_name;
	log_data->path = UTL_POSITIVE_PATH;

	if (!temp_function_name(log_data, "alpha transition test"))
	{
		return false;
	}
	UTL_log_start_test_path(log_data);

	test_passed = core_alpha_character_transition_unit_test(log_data, current_state,
		get_transition_character_type);

	UTL_log_end_test_path(log_data);

	free((void*)log_data->function_name);
	log_data->function_name = real_function_name;

	return test_passed;
}

static bool unit_test_punctuation_transition(UTL_Test_Log_Data* log_data, const LAH_Syntax_State current_state)
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

	const char* real_function_name = log_data->function_name;
	if (!temp_function_name(log_data, "punctuation transition test"))
	{
		return false;
	}

	if (core_non_alpha_character_transition_unit_test(log_data, current_state,
		input, expected_transition, positive_path_count))
	{
		test_passed = log_data->status;
	}

	free((void *)log_data->function_name);

	log_data->function_name = real_function_name;
	log_data->status = test_passed;

	return test_passed;
}

typedef bool (*character_transition_test_function)(UTL_Test_Log_Data* log_data, const LAH_Syntax_State state);

bool unit_test_get_transition_character_type(const size_t test_step)
{
	bool test_passed = true;

	UTL_Test_Log_Data* log_data = UTL_create_and_init_test_log_data(
		"unit_test_get_transition_character_type", test_passed, UTL_POSITIVE_PATH,
		test_step == 0, true);
	if (!log_data)
	{
		UTL_report_create_and_init_test_log_data_memory_failure(
			"unit_test_get_transition_character_type");
		return false;
	}

	UTL_va_test_log_fprintf(log_data, false,
		"STARTING internal unit test for get_transition_character_type("
		"unsigned char input, Syntax_State current_state)\n");

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

	UTL_va_test_log_fprintf(log_data, false,
		"\nENDING internal unit test for get_transition_character_type("
		"unsigned char input, Syntax_State current_state)\n");

	free(log_data);

	return test_passed;
}
#endif	// INTERNAL_CHARACTER_TRANSITION_UNIT_TEST_C
