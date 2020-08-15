/* 
 * internal_sytax_state_tests.c
 *
 * This file contains internal syntax state machine unit tests, these test functions test
 * the very basic functions that are the building blocks of the public interface, they are
 * declared static so these tests must be included in the syntax_state_machine.c file rather
 * externally.
 */

#include "internal_sytax_state_tests.h"

typedef struct state_test_data
{
	Syntax_State current_state;
	Syntax_State expected_state;
	Syntax_Check_Offss check_list_item_to_update;
	unsigned check_list_item_prestate;
	unsigned expected_check_list_item_value;
	unsigned check_list_item_max_value;
} State_Test_Data;

typedef Syntax_State(*Function_Under_Test_3_args)(Syntax_State current_state, Syntax_State_Transition* next_states, unsigned syntax_check_list[]);

#ifdef UNIT_TEST_DEBUG
static char *state_name_for_printing(Syntax_State state)
{
	return debug_state_names[(size_t)state];
}
#endif

static char* transition_character[] =
{
	"Transition on {",
	"Transition on }",
	"Transition on ,",
	"Transition on Alpha",
	"Transition on Digit",
	"Transition on White Space",
	"Transition on EOL",
};

static bool unit_test_syntax_states(void)
{
	bool test_passed = true;

	Syntax_State_Transition* test_transitions = create_next_states();

	for (size_t state = 0; state < SYNTAX_STATE_ARRAY_SIZE; state++)
	{
		char out_buffer[BUFSIZ];
		sprintf(out_buffer, "current_state = %s\n", state_name_for_printing(test_transitions[state].current_state));
		log_generic_message(out_buffer);

		for (size_t character_index = 0; character_index < TRANSITION_ARRAY_SIZE; character_index++)
		{
			sprintf(out_buffer, "\ttransition character = %s\t\tnew state %s\n", transition_character[character_index],
				state_name_for_printing(test_transitions[state].transition_on_char_type[character_index]));
			log_generic_message(out_buffer);
		}
		log_generic_message("\n");
	}

	return test_passed;
}

static void log_all_failure_data_for_unit_test_execute_transitions_all_states_special_characters(
	Test_Log_Data log_data, State_Test_Data test_data[], unsigned syntax_check_list[],
	Syntax_State current_state, Syntax_State new_state, size_t test_count)
{
	log_test_status_each_step2(&log_data);

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

static bool unit_test_execute_transitions_all_states_special_characters(Function_Under_Test_3_args test_function,
	Test_Log_Data log_data, size_t positive_path_test_count, State_Test_Data test_data[], size_t test_runs)
{
	bool test_passed = true;

	Syntax_State_Transition* next_states = create_next_states();
	if (!next_states)
	{
		fprintf(error_out_file, "In %s create_next_states(), failed, exiting function.\n", log_data.function_name);
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
				log_data, test_data, syntax_check_list, current_state, new_state, test_count);
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

	free(next_states);

	return test_passed;
}

static bool unit_test_state_transition_on_closebrace(unsigned test_step)
{
	bool test_passed = true;
	State_Test_Data test_data[] =
	{
		// Start with positive test path data
		{OPERAND_STATE, END_STATEMENT_STATE, CLOSEBRACE, 0, MAX_CLOSE_BRACE, MAX_CLOSE_BRACE},
		// Negative test path data
		{START_STATE, ERROR_STATE, CLOSEBRACE, 0, 1, MAX_CLOSE_BRACE},
		{ENTER_OPCODE_STATE, END_STATEMENT_STATE, CLOSEBRACE, 0, 1, MAX_CLOSE_BRACE},
		{OPCODE_STATE, END_STATEMENT_STATE, CLOSEBRACE, 0, 1, MAX_CLOSE_BRACE},
		{END_OPCODE_STATE, END_STATEMENT_STATE, CLOSEBRACE, 0, 1, MAX_CLOSE_BRACE},
		{ENTER_OPERAND_STATE, END_STATEMENT_STATE, CLOSEBRACE, 0, MAX_CLOSE_BRACE, MAX_CLOSE_BRACE},
		{END_OPERAND_STATE, END_STATEMENT_STATE, CLOSEBRACE, 0, MAX_CLOSE_BRACE, MAX_CLOSE_BRACE},
		{END_STATEMENT_STATE, END_STATEMENT_STATE, CLOSEBRACE, 1, 2, MAX_CLOSE_BRACE},
		{DONE_STATE, ERROR_STATE, CLOSEBRACE, 1, 2, MAX_CLOSE_BRACE},
		{END_OPERAND_STATE, END_STATEMENT_STATE, CLOSEBRACE, MAX_CLOSE_BRACE, MAX_CLOSE_BRACE + 1, MAX_CLOSE_BRACE}		// Positive, except for 2 close braces so negative
	};

	size_t positive_path_test_count = 1;

	Test_Log_Data log_data = { "unit_test_state_transition_on_closebrace", test_passed, "Positive", test_step == 0};

	test_passed = unit_test_execute_transitions_all_states_special_characters(state_transition_on_closebrace, log_data, positive_path_test_count, test_data,
		sizeof(test_data)/sizeof(*test_data));

	return test_passed;
}

static bool unit_test_state_transition_on_openbrace(unsigned test_step)
{
	bool test_passed = true;
	State_Test_Data test_data[] =
	{
		// Start with positive test path data
		{START_STATE, ENTER_OPCODE_STATE, OPENBRACE, 0, MAX_OPEN_BRACE, MAX_OPEN_BRACE},
		// Negative test path data
		{ENTER_OPCODE_STATE, ENTER_OPCODE_STATE, OPENBRACE, 1, 2, MAX_OPEN_BRACE},
		{OPCODE_STATE, ERROR_STATE, OPENBRACE, 1, 2, MAX_OPEN_BRACE},
		{END_OPCODE_STATE, ERROR_STATE, OPENBRACE, 1, 2, MAX_OPEN_BRACE},
		{ENTER_OPERAND_STATE, ERROR_STATE, OPENBRACE, 1, 2, MAX_OPEN_BRACE},
		{OPERAND_STATE, ERROR_STATE, OPENBRACE, 1, 2, MAX_OPEN_BRACE},
		{END_OPERAND_STATE, ERROR_STATE, OPENBRACE, 1, 2, MAX_OPEN_BRACE},
		{END_STATEMENT_STATE, ERROR_STATE, OPENBRACE, 1, 2, MAX_OPEN_BRACE},
		{DONE_STATE, ERROR_STATE, OPENBRACE, 1, 2, MAX_OPEN_BRACE},
		{ERROR_STATE, ERROR_STATE, OPENBRACE, 1, 2, MAX_OPEN_BRACE},
	};

	size_t positive_path_test_count = 1;

	Test_Log_Data log_data = { "unit_test_state_transition_on_openbrace", test_passed, "Positive", test_step == 0 };

	test_passed = unit_test_execute_transitions_all_states_special_characters(state_transition_on_openbrace, log_data, positive_path_test_count, test_data,
		sizeof(test_data) / sizeof(*test_data));

	return test_passed;
}

static bool unit_test_state_transition_on_comma(unsigned test_step)
{
	bool test_passed = true;
	State_Test_Data test_data[] =
	{
		// Start with positive test path data
		{END_STATEMENT_STATE, DONE_STATE, COMMA, 1, 2, MAX_COMMA},
		{OPCODE_STATE, ENTER_OPERAND_STATE, COMMA, 0, 1, MAX_COMMA},
		{END_OPCODE_STATE, ENTER_OPERAND_STATE, COMMA, 0, 1, MAX_COMMA},
		// Negative test path data
		{START_STATE, ENTER_OPERAND_STATE, COMMA, 0, 1, MAX_COMMA},
		{ENTER_OPCODE_STATE, ENTER_OPERAND_STATE, COMMA, 0, 1, MAX_COMMA},
		{ENTER_OPERAND_STATE, DONE_STATE, COMMA, 1, MAX_COMMA, MAX_COMMA},
		{OPERAND_STATE, DONE_STATE, COMMA, 1, MAX_COMMA, MAX_COMMA},
		{END_OPERAND_STATE, DONE_STATE, COMMA, 1, MAX_COMMA, MAX_COMMA},
		{ERROR_STATE, ERROR_STATE, COMMA, 0, 1, MAX_COMMA},
	};

	size_t positive_path_test_count = 3;

	Test_Log_Data log_data = { "unit_test_state_transition_on_comma", test_passed, "Positive", test_step == 0 };

	test_passed = unit_test_execute_transitions_all_states_special_characters(state_transition_on_comma, log_data, positive_path_test_count, test_data,
		sizeof(test_data) / sizeof(*test_data));

	return test_passed;
}

bool internal_tests_on_all_state_transitions(unsigned test_step)
{
	bool all_tests_passed = true;

	all_tests_passed = unit_test_syntax_states();

	all_tests_passed = unit_test_state_transition_on_closebrace(test_step);

	all_tests_passed = unit_test_state_transition_on_openbrace(test_step);

	all_tests_passed = unit_test_state_transition_on_comma(test_step);

	return all_tests_passed;
}