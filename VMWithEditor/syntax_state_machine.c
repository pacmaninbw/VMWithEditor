/*
 * syntax_state_machine.c
 */
#include "syntax_state_machine.h"
#include "virtual_machine.h"
#ifdef UNIT_TESTING
#include "common_unit_test_logic.h"
#else
#include "common_program_logic.h"
#endif
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void init_next_states(Syntax_State_Transition next_states[(int)ERROR_STATE])
{
	next_states[START_STATE] = (Syntax_State_Transition){ ENTER_OPCODE_STATE, OPCODE_STATE };
	next_states[ENTER_OPCODE_STATE] = (Syntax_State_Transition){ OPCODE_STATE, END_OPCODE_STATE };
	next_states[OPCODE_STATE] = (Syntax_State_Transition){ END_OPCODE_STATE, ENTER_OPERAND_STATE };
	next_states[END_OPCODE_STATE] = (Syntax_State_Transition){ ENTER_OPERAND_STATE, OPERAND_STATE };
	next_states[ENTER_OPERAND_STATE] = (Syntax_State_Transition){ OPERAND_STATE, END_OPERAND_STATE };
	next_states[OPERAND_STATE] = (Syntax_State_Transition){ END_OPERAND_STATE, END_STATEMENT };
	next_states[END_OPERAND_STATE] = (Syntax_State_Transition){ END_STATEMENT, DONE_STATE };
	next_states[END_STATEMENT] = (Syntax_State_Transition){ DONE_STATE, ERROR_STATE };
	next_states[DONE_STATE] = (Syntax_State_Transition){ DONE_STATE, ERROR_STATE };
	next_states[ERROR_STATE] = (Syntax_State_Transition){ ERROR_STATE, ERROR_STATE };
}

static Syntax_State state_transition_on_closebrace(Syntax_State current_state, Syntax_State_Transition next_states[], unsigned syntax_check_list[])
{
	switch (current_state)
	{
	case OPERAND_STATE:
	case END_OPERAND_STATE:
		syntax_check_list[CLOSEBRACE]++;
		if (syntax_check_list[CLOSEBRACE] >= MAX_CLOSE_BRACE)
		{
			syntax_check_list[MULTIPLESTATEMENTSONELINE]++;
		}
		return next_states[current_state].legal_next_state;

	default:
		return ERROR_STATE;
	}
}

static Syntax_State state_transition_on_comma(Syntax_State current_state, Syntax_State_Transition next_states[], unsigned syntax_check_list[])
{
	switch (current_state)
	{
		case OPCODE_STATE:
		case END_STATEMENT:
			return next_states[current_state].legal_next_state;

		case END_OPCODE_STATE:
		case END_OPERAND_STATE:
		case ENTER_OPCODE_STATE:
			return next_states[current_state].error_with_next_state;

		case DONE_STATE:
			if (syntax_check_list[COMMA] < MAX_COMMA)
			{
				syntax_check_list[COMMA]++;
				return next_states[current_state].legal_next_state;
			}
			else
			{
				return next_states[current_state].error_with_next_state;
			}

		case START_STATE:
			syntax_check_list[ILLEGALFIRSTCHAR]++;
			return OPERAND_STATE;

		default:
			return ERROR_STATE;
	}
}

static Syntax_State state_transition_on_openbrace(Syntax_State current_state, Syntax_State_Transition next_states[], unsigned syntax_check_list[])
{
	switch (current_state)
	{
		case START_STATE:
			syntax_check_list[OPENBRACE]++;
			if (syntax_check_list[OPENBRACE] >= MAX_CLOSE_BRACE)
			{
				syntax_check_list[MULTIPLESTATEMENTSONELINE]++;
			}
			return next_states[current_state].legal_next_state;

		default:
			return ERROR_STATE;
	}
}

static Syntax_State state_transition_on_end_of_line(Syntax_State current_state, Syntax_State_Transition next_states[])
{
	switch (current_state)
	{
	case START_STATE:		// Blank line is ok
	case DONE_STATE:
		return DONE_STATE;
	case END_STATEMENT:
	case END_OPERAND_STATE:
		return next_states[current_state].error_with_next_state;
	default:
		return ERROR_STATE;
	}
}

static Syntax_State state_transition_on_white_space(Syntax_State current_state, Syntax_State_Transition next_states[])
{
	switch (current_state)
	{
	case OPCODE_STATE:
	case OPERAND_STATE:
		return next_states[current_state].legal_next_state;
		break;
	default:
		return current_state;
	}
}

static bool is_legal_in_hex_number(unsigned char input)
{
	bool is_legal = false;
	char legal_hex_characters[] = "XABCDEF";
	char* legal_char_ptr = legal_hex_characters;

	input = (char)toupper(input);

	while (*legal_char_ptr)
	{
		if (input == *legal_char_ptr)
		{
			return true;
		}
		legal_char_ptr++;
	}

	return is_legal;
}

static Syntax_State state_transition_on_alpha(Syntax_State current_state, Syntax_State_Transition next_states[], unsigned char input, unsigned syntax_check_list[])
{
	switch (current_state)
	{
	case START_STATE:
		syntax_check_list[ILLEGALFIRSTCHAR]++;
		return next_states[current_state].error_with_next_state;

	case ENTER_OPCODE_STATE:
		return next_states[current_state].legal_next_state;

	case OPCODE_STATE:
		return current_state;

	case OPERAND_STATE:
		if (is_legal_in_hex_number(input))
		{
			return current_state;
		}
		else
		{
			return ERROR_STATE;
		}
		break;

	default:
		return ERROR_STATE;
	}
}

static Syntax_State state_transition_on_digit(Syntax_State current_state, Syntax_State_Transition next_states[], unsigned syntax_check_list[])
{
	switch (current_state)
	{
	case OPERAND_STATE:
		return current_state;

	case ENTER_OPERAND_STATE:
		return next_states[current_state].legal_next_state;

	case END_OPCODE_STATE:
		return next_states[current_state].error_with_next_state;

	case START_STATE:
		syntax_check_list[ILLEGALFIRSTCHAR]++;
	default:
		return ERROR_STATE;
	}
}

Syntax_State state_transition(Syntax_State current_state, unsigned char* input, unsigned syntax_check_list[])
{
	Syntax_State_Transition next_states[(int)ERROR_STATE];
	init_next_states(next_states);

	if (*input == '\n')
	{
		return state_transition_on_end_of_line(current_state, next_states);
	}

	if (isspace(*input))
	{
		return state_transition_on_white_space(current_state, next_states);
	}

	if (isalpha(*input))
	{
		return state_transition_on_alpha(current_state, next_states, *input, syntax_check_list);
	}

	if (isdigit(*input))
	{
		return state_transition_on_digit(current_state, next_states, syntax_check_list);
	}

	if (*input == ',')
	{
		return state_transition_on_comma(current_state, next_states, syntax_check_list);
	}

	if (*input == '{')
	{
		return state_transition_on_openbrace(current_state, next_states, syntax_check_list);
	}

	if (*input == '}')
	{
		return state_transition_on_closebrace(current_state, next_states, syntax_check_list);
	}

	return ERROR_STATE;
}

