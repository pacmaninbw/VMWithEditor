#include "lexical_analyzer_unit_test_utilities.h"
#include "common_unit_test_logic.h"
#include <stdio.h>

const char* state_name_for_printing(const LAH_Syntax_State state)
{
	char* state_names[LAH_SYNTAX_STATE_ARRAY_SIZE] =
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

const char* transition_names(const LAH_State_Transition_Characters transition)
{
	static char* transition_character[LAH_TRANSITION_ARRAY_SIZE] =
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


