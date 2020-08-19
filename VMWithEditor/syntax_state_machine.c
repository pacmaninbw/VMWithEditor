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
 *  ILLEGAL_CHAR_TRANSITION = 7
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
		ENTER_OPERAND_STATE, OPCODE_STATE, OPERAND_STATE, START_STATE, DONE_STATE, ERROR_STATE} };
	next_states[ENTER_OPCODE_STATE] = (Syntax_State_Transition){ ENTER_OPCODE_STATE, {ENTER_OPCODE_STATE,
		END_STATEMENT_STATE, ENTER_OPERAND_STATE, OPCODE_STATE, OPERAND_STATE, ENTER_OPCODE_STATE,
		ERROR_STATE, ERROR_STATE} };
	next_states[OPCODE_STATE] = (Syntax_State_Transition){OPCODE_STATE, {ERROR_STATE, END_STATEMENT_STATE,
		ENTER_OPERAND_STATE, OPCODE_STATE, OPERAND_STATE, END_OPCODE_STATE, ERROR_STATE, ERROR_STATE} };
	next_states[END_OPCODE_STATE] = (Syntax_State_Transition){ END_OPCODE_STATE, {ERROR_STATE,
		END_STATEMENT_STATE, ENTER_OPERAND_STATE, ERROR_STATE, OPERAND_STATE, END_OPCODE_STATE,
		ERROR_STATE, ERROR_STATE} };
	next_states[ENTER_OPERAND_STATE] = (Syntax_State_Transition){ ENTER_OPERAND_STATE, {ERROR_STATE,
		END_STATEMENT_STATE, DONE_STATE, ERROR_STATE, OPERAND_STATE, ENTER_OPERAND_STATE, ERROR_STATE} };
	next_states[OPERAND_STATE] = (Syntax_State_Transition){ OPERAND_STATE, {ERROR_STATE, END_STATEMENT_STATE,
		DONE_STATE, ERROR_STATE, OPERAND_STATE, END_OPERAND_STATE, ERROR_STATE, ERROR_STATE} };
	next_states[END_OPERAND_STATE] = (Syntax_State_Transition){ END_OPERAND_STATE, {ERROR_STATE,
		END_STATEMENT_STATE, DONE_STATE, ERROR_STATE, ERROR_STATE, END_OPERAND_STATE, ERROR_STATE, ERROR_STATE} };
	next_states[END_STATEMENT_STATE] = (Syntax_State_Transition){ END_STATEMENT_STATE, {ERROR_STATE,
		END_STATEMENT_STATE, DONE_STATE, ERROR_STATE, ERROR_STATE, END_STATEMENT_STATE, DONE_STATE, ERROR_STATE} };
	next_states[DONE_STATE] = (Syntax_State_Transition){ DONE_STATE, {ERROR_STATE, ERROR_STATE,
		DONE_STATE, ERROR_STATE, ERROR_STATE, DONE_STATE, DONE_STATE, ERROR_STATE} };
	next_states[ERROR_STATE] = (Syntax_State_Transition){ ERROR_STATE, {ERROR_STATE, ERROR_STATE,
		ERROR_STATE, ERROR_STATE, ERROR_STATE, ERROR_STATE, ERROR_STATE, ERROR_STATE} };

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

static bool is_legal_in_hex_number(unsigned char input)
{
	bool is_legal = false;

	switch (toupper(input))
	{
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'X':
			is_legal = true;
			break;

		default:
			is_legal = false;
			break;
	}


	return is_legal;
}

/*
 * The calling function has already gone through one filter so it is assured that
 * the input character is an alpha and not some other type of character.
 */
static State_Transition_Characters get_alpha_input_transition_character_type(unsigned char input, Syntax_State current_state)
{
	State_Transition_Characters character_type = ILLEGAL_CHAR_TRANSITION;

	switch (current_state)
	{
		case ENTER_OPERAND_STATE:
		case OPERAND_STATE:
		case END_OPERAND_STATE:
			character_type = (is_legal_in_hex_number(input)) ? DIGIT_STATE_TRANSITION :
				ALPHA_STATE_TRANSITION;
			break;

		default:
			character_type = ALPHA_STATE_TRANSITION;
			break;
	}

	return character_type;
}

/*
 * The calling function has already gone through several filter so it is assured
 * that the input character is not an alpha, digit, white space or end of line.
 */
static State_Transition_Characters get_puctuation_transition_character_type(unsigned char input)
{
	State_Transition_Characters character_type = ILLEGAL_CHAR_TRANSITION;

	switch (input)
	{
		case ',':
			character_type = COMMA_STATE_TRANSITION;
			break;

		case '{':
			character_type = OPENBRACE_STATE_TRANSITION;
			break;

		case '}':
			character_type = CLOSEBRACE_STATE_TRANSITION;
			break;

		default:
			character_type = ILLEGAL_CHAR_TRANSITION;
			break;
	}

	return character_type;
}

/*
 * Rather than create a table indexed by each and every character in the character
 * set save space using ctype functions for large ranges. Also save time on
 * implementation and debugging.
 */
State_Transition_Characters get_transition_character_type(unsigned char input, Syntax_State current_state)
{
	State_Transition_Characters character_type = ILLEGAL_CHAR_TRANSITION;
	if (isalpha(input))
	{
		character_type = get_alpha_input_transition_character_type(input, current_state);
	}
	else if (isdigit(input))
	{
		character_type = DIGIT_STATE_TRANSITION;
	}
	else if (isspace(input))
	{
		switch (input)
		{
			case ' ' :
			case '\t' :
				character_type = WHITESPACE_STATE_TRANSITION;
				break;

			case '\n' :
			case '\r' :
				character_type = EOL_STATE_TRANSITION;
				break;

			default:
				character_type = ILLEGAL_CHAR_TRANSITION;
				break;
		}
	}
	else
	{
		character_type = get_puctuation_transition_character_type(input);
	}

	return character_type;
}

/*
 * syntax_check_list provides additional error information for the parser.
 */
static void collect_error_reporting_data(Syntax_State current_state,
	State_Transition_Characters character_type, unsigned syntax_check_list[])
{
	switch (character_type)
	{
		case WHITESPACE_STATE_TRANSITION:		// This section is for character types that
		case EOL_STATE_TRANSITION:				// are a legal first character on a line
			break;

		case COMMA_STATE_TRANSITION:			// Punctuation required by grammer on
		case OPENBRACE_STATE_TRANSITION:		// every line
		case CLOSEBRACE_STATE_TRANSITION:
		{
			unsigned maximum_allow[] = { MAX_OPEN_BRACE, MAX_CLOSE_BRACE, MAX_COMMA };
			syntax_check_list[character_type]++;
			if (syntax_check_list[character_type] > maximum_allow[character_type])
			{
				syntax_check_list[MULTIPLESTATEMENTSONELINE]++;
			}
		}	// flow through so that punctuation is handeled like all other character
		default:
			if (current_state == START_STATE && character_type != OPENBRACE_STATE_TRANSITION)
			{
				syntax_check_list[ILLEGALFIRSTCHAR]++;
			}
			break;
	}
}

Syntax_State state_transition_collect_parser_error_data(Syntax_State current_state, unsigned char* input, unsigned syntax_check_list[])
{
	State_Transition_Characters character_type = get_transition_character_type(*input, current_state);

	collect_error_reporting_data(current_state, character_type, syntax_check_list);

	Syntax_State new_state = ERROR_STATE;
	Syntax_State_Transition* next_states = create_next_states();
	if (next_states)
	{
		new_state = next_states[current_state].transition_on_char_type[character_type];
		free(next_states);
	}

	return new_state;
}


#ifdef UNIT_TESTING
#include "internal_sytax_state_tests.c"
#endif
