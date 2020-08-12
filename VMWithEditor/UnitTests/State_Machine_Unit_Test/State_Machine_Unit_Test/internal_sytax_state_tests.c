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

static bool unit_test_state_transition_on_closebrace(unsigned test_step)
{
	bool test_passed = true;
	State_Test_Data test_data[] =
	{
		// Start with positive test path data
		{OPERAND_STATE, END_OPCODE_STATE, CLOSEBRACE, 0, 1},
		{ENTER_OPERAND_STATE, END_OPCODE_STATE, CLOSEBRACE, 0, 1},
		{END_OPERAND_STATE, END_STATEMENT_STATE, CLOSEBRACE, 0, 1},		// will have error messages 
		// Negative test path data
		{START_STATE, ERROR_STATE, CLOSEBRACE, 0, 1},
		{ENTER_OPCODE_STATE, ERROR_STATE, CLOSEBRACE, 0, 1},
		{OPCODE_STATE, ERROR_STATE, CLOSEBRACE, 0, 1},
		{END_OPCODE_STATE, ERROR_STATE, CLOSEBRACE, 0, 1},
		{END_OPERAND_STATE, ERROR_STATE, CLOSEBRACE, 0, 1},
		{END_STATEMENT_STATE, ERROR_STATE, CLOSEBRACE, 1, 2},
		{DONE_STATE, ERROR_STATE, CLOSEBRACE, 1, 2},
		{END_OPERAND_STATE, END_STATEMENT_STATE, CLOSEBRACE, 1, 2}		// Positive, except for 2 close braces so negative
	};

	size_t positive_count = 3;

	unsigned syntax_check_list[SYNTAX_CHECK_COUNT];

	Syntax_State_Transition next_states[(int)ERROR_STATE];
	init_next_states(next_states);


	for (size_t test_count = 0; test_count <= (sizeof(test_data) / sizeof(*test_data)); test_count++)
	{
		bool current_test_passed = true;
		memset(&syntax_check_list[0], 0, sizeof(syntax_check_list));
		syntax_check_list[test_data[test_count].check_list_item_to_update] = test_data[test_count].check_list_item_prestate;
		Syntax_State current_state = test_data[test_count].current_state;
		Syntax_State new_state = state_transition_on_closebrace(current_state, next_states, syntax_check_list);

		if (new_state != test_data[test_count].expected_state
			|| syntax_check_list[test_data[test_count].check_list_item_to_update] != test_data[test_count].expected_check_list_item_value
			|| (test_data[test_count].check_list_item_prestate && !syntax_check_list[MULTIPLESTATEMENTSONELINE]))
		{
			test_passed = false;
		}
	}

	return test_passed;
}

bool internal_tests_on_all_state_transitions(unsigned test_step)
{
	bool all_tests_passed = true;

	all_tests_passed = unit_test_state_transition_on_closebrace(test_step);

	return all_tests_passed;
}