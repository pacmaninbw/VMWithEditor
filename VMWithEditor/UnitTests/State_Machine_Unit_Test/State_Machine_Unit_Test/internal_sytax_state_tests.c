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
} State_Test_Data;

typedef bool (*UTF_3_args)(Syntax_State current_state, Syntax_State_Transition* next_states, unsigned syntax_check_list[]);

static bool unit_test_execute_transitions_all_states_special_characters(UTF_3_args test_function,
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
			|| (test_data[test_count].check_list_item_prestate && !syntax_check_list[MULTIPLESTATEMENTSONELINE])
			|| (!test_data[test_count].check_list_item_prestate && syntax_check_list[MULTIPLESTATEMENTSONELINE]))
		{
			log_data.status = false;
		}

		log_test_status_each_step2(&log_data);

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
		{OPERAND_STATE, END_OPERAND_STATE, CLOSEBRACE, 0, 1},
		{ENTER_OPERAND_STATE, END_OPERAND_STATE, CLOSEBRACE, 0, 1},
		{END_OPERAND_STATE, END_STATEMENT_STATE, CLOSEBRACE, 0, 1},
		// Negative test path data
		{START_STATE, ERROR_STATE, CLOSEBRACE, 0, 1},
		{ENTER_OPCODE_STATE, ERROR_STATE, CLOSEBRACE, 0, 1},
		{OPCODE_STATE, ERROR_STATE, CLOSEBRACE, 0, 1},
		{END_OPCODE_STATE, ERROR_STATE, CLOSEBRACE, 0, 1},
		{END_STATEMENT_STATE, ERROR_STATE, CLOSEBRACE, 1, 2},
		{DONE_STATE, ERROR_STATE, CLOSEBRACE, 1, 2},
		{END_OPERAND_STATE, END_STATEMENT_STATE, CLOSEBRACE, 1, 2}		// Positive, except for 2 close braces so negative
	};

	size_t positive_path_test_count = 3;

	char *function_name = "unit_test_state_transition_on_closebrace";
	Test_Log_Data log_data = { "unit_test_state_transition_on_closebrace", test_passed, "Positive", test_step == 0};

	test_passed = unit_test_execute_transitions_all_states_special_characters(state_transition_on_closebrace, log_data, positive_path_test_count, test_data,
		sizeof(test_data)/sizeof(*test_data));

	return test_passed;
}

bool internal_tests_on_all_state_transitions(unsigned test_step)
{
	bool all_tests_passed = true;

	all_tests_passed = unit_test_state_transition_on_closebrace(test_step);

	return all_tests_passed;
}