#include "lexical_analyzer_unit_test_utilities.h"

const char* state_name_for_printing(const Syntax_State state)
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

const char* transition_names(const State_Transition_Characters transition)
{
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

	return transition_character[(size_t)transition];
}

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
