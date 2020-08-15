/*
 * syntax_state_machine.c
 *
 * The Syntax State Machine is a simple lexical analiser. Given the current syntax
 * state and the new input character what is the new syntax state. State machines
 * can be represented as tables. Table implementation of a state machine uses
 * more memory but performs faster, the lexical analyser programs Flex and LEX
 * generate tables to implement lexical analysis.
 *
 * This module uses enums to make the states and transitions easier to understand.
 *
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

/*
 * This function returns the table that represents the current syntactic state
 * and the new state that each possible legal into can go to from the current
 * state.
 *
 * To allow the parser to report as many errors as possible per statement
 * not all errors result in ERROR_STATE, missing required items are reported
 * in a separate data structure. The decision to report the error is made
 * at the parser level.
 *
 * Columns in table below
 *	OPENBRACE_STATE_TRANSITION = 0,
 *	CLOSEBRACE_STATE_TRANSITION = 1,
 *	COMMA_STATE_TRANSITION = 2,
 *	ALPHA_STATE_TRANSITION = 3,
 *	DIGIT_STATE_TRANSITION = 4,
 *	WHITESPACE_STATE_TRANSITION = 5,
 *	EOL_STATE_TRANSITION = 6		// End of Line
 *
 * Rows in table below
 *	START_STATE = 0,			Start of a new line, only white space or open brace is really expected
 *	ENTER_OPCODE_STATE = 1,		Open brace encountered, waiting for opcode (first alpha character) white space or alpha is expected
 *	OPCODE_STATE = 2,			Open brace and first leter of opcode have been encoutered more alpha, white space or comma expected
 *	END_OPCODE_STATE = 3,		White space has been encountered only white space or comma expected
 *	ENTER_OPERAND_STATE = 4,	Comma has been encountered, waiting for first digit of operand white space allowed
 *	OPERAND_STATE = 5,			First digit of operand has been encountered, remain in this state until white space or close brace is encountered.
 *	END_OPERAND_STATE = 6,		White space has been encountered, waiting for close brace to end statement
 *	END_STATEMENT_STATE = 7,	Close brace has been encountered, comma or new line expected
 *	DONE_STATE = 8,				Comma has been encountered only legal input is white space or new line
 *	ERROR_STATE = 9
 */
static Syntax_State_Transition* create_next_states(void)
{
	Syntax_State_Transition *next_states = calloc(((size_t)ERROR_STATE) + 1, sizeof(*next_states));
	if (!next_states)
	{
		report_error_generic("In create_next_states(), memory allocation for next_states failed\n");
		return next_states;
	}

	next_states[START_STATE] = (Syntax_State_Transition){ START_STATE, {ENTER_OPCODE_STATE, ERROR_STATE, 
		ENTER_OPERAND_STATE, OPCODE_STATE, OPERAND_STATE, START_STATE, DONE_STATE} };
	next_states[ENTER_OPCODE_STATE] = (Syntax_State_Transition){ ENTER_OPCODE_STATE, {ENTER_OPCODE_STATE,
		END_STATEMENT_STATE, ENTER_OPERAND_STATE, OPCODE_STATE, OPERAND_STATE, ENTER_OPCODE_STATE,
		ERROR_STATE} };
	next_states[OPCODE_STATE] = (Syntax_State_Transition){OPCODE_STATE, {ERROR_STATE, END_STATEMENT_STATE,
		ENTER_OPERAND_STATE, OPCODE_STATE, OPERAND_STATE, END_OPCODE_STATE, ERROR_STATE} };
	next_states[END_OPCODE_STATE] = (Syntax_State_Transition){ END_OPCODE_STATE, {ERROR_STATE,
		END_STATEMENT_STATE, ENTER_OPERAND_STATE, ERROR_STATE, OPERAND_STATE, END_OPCODE_STATE,
		ERROR_STATE} };
	next_states[ENTER_OPERAND_STATE] = (Syntax_State_Transition){ ENTER_OPERAND_STATE, {ERROR_STATE,
		END_STATEMENT_STATE, DONE_STATE, ERROR_STATE, OPERAND_STATE, ENTER_OPERAND_STATE, ERROR_STATE} };
	next_states[OPERAND_STATE] = (Syntax_State_Transition){ OPERAND_STATE, {ERROR_STATE, END_STATEMENT_STATE,
		DONE_STATE, ERROR_STATE, OPERAND_STATE, END_OPERAND_STATE, ERROR_STATE} };
	next_states[END_OPERAND_STATE] = (Syntax_State_Transition){ END_OPERAND_STATE, {ERROR_STATE,
		END_STATEMENT_STATE, DONE_STATE, ERROR_STATE, ERROR_STATE, END_OPERAND_STATE, ERROR_STATE} };
	next_states[END_STATEMENT_STATE] = (Syntax_State_Transition){ END_STATEMENT_STATE, {ERROR_STATE,
		END_STATEMENT_STATE, DONE_STATE, ERROR_STATE, ERROR_STATE, END_STATEMENT_STATE, DONE_STATE} };
	next_states[DONE_STATE] = (Syntax_State_Transition){ DONE_STATE, {ERROR_STATE, ERROR_STATE,
		DONE_STATE, ERROR_STATE, ERROR_STATE, DONE_STATE, DONE_STATE} };
	next_states[ERROR_STATE] = (Syntax_State_Transition){ ERROR_STATE, {ERROR_STATE, ERROR_STATE,
		ERROR_STATE, ERROR_STATE, ERROR_STATE, ERROR_STATE, ERROR_STATE} };

	return next_states;
}

static Syntax_State state_transition_on_closebrace(Syntax_State current_state, Syntax_State_Transition *next_states, unsigned syntax_check_list[])
{
	Syntax_State new_state = (current_state == ERROR_STATE) ? ERROR_STATE : next_states[current_state].transition_on_char_type[CLOSEBRACE_STATE_TRANSITION];

	syntax_check_list[CLOSEBRACE]++;
	if (syntax_check_list[CLOSEBRACE] > MAX_CLOSE_BRACE)
	{
		syntax_check_list[MULTIPLESTATEMENTSONELINE]++;
	}

	return new_state;
}

static Syntax_State state_transition_on_comma(Syntax_State current_state, Syntax_State_Transition *next_states, unsigned syntax_check_list[])
{
	Syntax_State new_state = (current_state == ERROR_STATE) ? ERROR_STATE : next_states[current_state].transition_on_char_type[COMMA_STATE_TRANSITION];

	syntax_check_list[COMMA]++;
	if (syntax_check_list[COMMA] > MAX_COMMA)
	{
		syntax_check_list[MULTIPLESTATEMENTSONELINE]++;
	}

	return new_state;
}

static Syntax_State state_transition_on_openbrace(Syntax_State current_state, Syntax_State_Transition *next_states, unsigned syntax_check_list[])
{
	Syntax_State new_state = (current_state == ERROR_STATE) ? ERROR_STATE : next_states[current_state].transition_on_char_type[OPENBRACE_STATE_TRANSITION];

	syntax_check_list[OPENBRACE]++;
	if (syntax_check_list[OPENBRACE] > MAX_CLOSE_BRACE)
	{
		syntax_check_list[MULTIPLESTATEMENTSONELINE]++;
	}

	return new_state;
}

static Syntax_State state_transition_on_end_of_line(Syntax_State current_state, Syntax_State_Transition *next_states)
{
	Syntax_State new_state = (current_state == ERROR_STATE) ? ERROR_STATE : next_states[current_state].transition_on_char_type[EOL_STATE_TRANSITION];

	return new_state;
}

static Syntax_State state_transition_on_white_space(Syntax_State current_state, Syntax_State_Transition *next_states)
{
	Syntax_State new_state = (current_state == ERROR_STATE) ? ERROR_STATE : next_states[current_state].transition_on_char_type[WHITESPACE_STATE_TRANSITION];

	return new_state;
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

static Syntax_State state_transition_on_alpha(Syntax_State current_state, Syntax_State_Transition *next_states, unsigned char input, unsigned syntax_check_list[])
{
	Syntax_State new_state = (current_state == ERROR_STATE) ? ERROR_STATE : next_states[current_state].transition_on_char_type[ALPHA_STATE_TRANSITION];

	switch (current_state)
	{
		case START_STATE:
			syntax_check_list[ILLEGALFIRSTCHAR]++;
			break;

		case OPERAND_STATE:
			if (is_legal_in_hex_number(input))
			{
				new_state = next_states[current_state].transition_on_char_type[DIGIT_STATE_TRANSITION];
			}
			break;
	}

	return new_state;
}

static Syntax_State state_transition_on_digit(Syntax_State current_state, Syntax_State_Transition *next_states, unsigned syntax_check_list[])
{
	Syntax_State new_state = (current_state == ERROR_STATE) ? ERROR_STATE : next_states[current_state].transition_on_char_type[ALPHA_STATE_TRANSITION];

	if (current_state == START_STATE)
	{
		syntax_check_list[ILLEGALFIRSTCHAR]++;
	}

	return new_state;
}

Syntax_State state_transition(Syntax_State current_state, unsigned char* input, unsigned syntax_check_list[])
{
	Syntax_State new_state = ERROR_STATE;
	Syntax_State_Transition *next_states = create_next_states();

	if (*input == '\n')
	{
		new_state =  state_transition_on_end_of_line(current_state, next_states);
	}

	if (isspace(*input))
	{
		new_state = state_transition_on_white_space(current_state, next_states);
	}

	if (isalpha(*input))
	{
		new_state = state_transition_on_alpha(current_state, next_states, *input, syntax_check_list);
	}

	if (isdigit(*input))
	{
		new_state = state_transition_on_digit(current_state, next_states, syntax_check_list);
	}

	if (*input == ',')
	{
		new_state = state_transition_on_comma(current_state, next_states, syntax_check_list);
	}

	if (*input == '{')
	{
		new_state = state_transition_on_openbrace(current_state, next_states, syntax_check_list);
	}

	if (*input == '}')
	{
		new_state = state_transition_on_closebrace(current_state, next_states, syntax_check_list);
	}

	free(next_states);

	return new_state;
}

#ifdef UNIT_TESTING
#include "internal_sytax_state_tests.c"
#endif
