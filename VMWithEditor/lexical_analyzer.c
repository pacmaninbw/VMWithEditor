#ifndef LEXICAL_ANALYZER_C
#define LEXICAL_ANALYZER_C

/*
 * lexical_analyzer.c
 *
 * The Syntax State Machine is a simple lexical analyzer. Given the current syntax
 * state and the new input character what is the new syntax state. State machines
 * can be represented as tables. Table implementation of a state machine uses
 * more memory but performs faster, the lexical analyser programs Flex and LEX
 * generate tables to implement lexical analysis.
 *
 * This module uses enums to make the states and transitions easier to understand.
 *
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vmwitheditor.h"
#include "ERH_error_reporting.h"
#include "LAH_lexical_analyzer.h"
#include "SSF_safe_string_functions.h"

/*
 * The calling function has already gone through one filter so it is assured that
 * the input character is an alpha and not some other type of character.
 */
static LAH_State_Transition_Characters get_alpha_input_transition_character_type(
	Const_U_Char input, const LAH_Syntax_State current_state)
{
	LAH_State_Transition_Characters character_type = LAH_ILLEGAL_CHAR_TRANSITION;

	switch (current_state)
	{
		case LAH_ENTER_OPERAND_STATE:
		case LAH_OPERAND_STATE:
		case LAH_END_OPERAND_STATE:
			character_type = (isxdigit(input) || (input == 'x' || input == 'X')) ?
				LAH_DIGIT_STATE_TRANSITION : LAH_ALPHA_STATE_TRANSITION;
			break;

		default:
			character_type = LAH_ALPHA_STATE_TRANSITION;
			break;
	}

	return character_type;
}

/*
 * The calling function has already gone through several filter so it is assured
 * that the input character is not an alpha, digit, white space or end of line.
 */
static LAH_State_Transition_Characters get_puctuation_transition_character_type(
	Const_U_Char input)
{
	LAH_State_Transition_Characters character_type = LAH_ILLEGAL_CHAR_TRANSITION;

	switch (input)
	{
		case ',':
			character_type = LAH_COMMA_STATE_TRANSITION;
			break;

		case '{':
			character_type = LAH_OPENBRACE_STATE_TRANSITION;
			break;

		case '}':
			character_type = LAH_CLOSEBRACE_STATE_TRANSITION;
			break;

		default:
			character_type = LAH_ILLEGAL_CHAR_TRANSITION;
			break;
	}

	return character_type;
}

/*
 * The calling function has already gone through several filter so it is assured
 * that the input character is not an alpha, digit, white space or end of line.
 */
static LAH_State_Transition_Characters get_whitespace_transition_character_type(
	Const_U_Char input)
{
	LAH_State_Transition_Characters character_type = LAH_ILLEGAL_CHAR_TRANSITION;

	switch (input)
	{
		case ' ':
		case '\t':
			character_type = LAH_WHITESPACE_STATE_TRANSITION;
			break;

		case '\n':
		case '\r':
			character_type = LAH_EOL_STATE_TRANSITION;
			break;

		default:
			character_type = LAH_ILLEGAL_CHAR_TRANSITION;
			break;
	}

	return character_type;
}

/*
 * Rather than create a table indexed by each and every character in the character
 * set save space using ctype functions for large ranges. Also save time on
 * implementation and debugging.
 */
static LAH_State_Transition_Characters get_transition_character_type(
	Const_U_Char input, const LAH_Syntax_State current_state)
{
	LAH_State_Transition_Characters character_type = LAH_ILLEGAL_CHAR_TRANSITION;
	if (isalpha(input))
	{
		character_type = get_alpha_input_transition_character_type(input, current_state);
	}
	else if (isdigit(input))
	{
		character_type = LAH_DIGIT_STATE_TRANSITION;
	}
	else if (isspace(input))
	{
		character_type = get_whitespace_transition_character_type(input);
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
static void collect_error_reporting_data(const LAH_Syntax_State current_state,
	const LAH_State_Transition_Characters character_type, unsigned syntax_check_list[])
{
	switch (character_type)
	{

		case LAH_WHITESPACE_STATE_TRANSITION:		// This section is for character types that
		case LAH_EOL_STATE_TRANSITION:				// are a legal first character on a line
			break;

		case LAH_COMMA_STATE_TRANSITION:			// Punctuation required by grammer on
		case LAH_OPENBRACE_STATE_TRANSITION:		// every line
		case LAH_CLOSEBRACE_STATE_TRANSITION:
		{
			unsigned maximum_allowed[] = { LAH_MAX_OPEN_BRACE, LAH_MAX_CLOSE_BRACE, LAH_MAX_COMMA };
			syntax_check_list[character_type]++;
			if (syntax_check_list[character_type] > maximum_allowed[character_type])
			{
				syntax_check_list[LAH_MULTIPLESTATEMENTSONELINE]++;
			}
		}	// flow through so that punctuation is handeled like all other character
		default:
			if (current_state == LAH_START_STATE && character_type != LAH_OPENBRACE_STATE_TRANSITION)
			{
				syntax_check_list[LAH_ILLEGALFIRSTCHAR]++;
			}
			break;
	}
}


#ifndef INCLUDED_IN_UNIT_TEST
 /*
  * This function returns the table that represents the current syntactic state
  * and the new state that each possible legal into can go to from the current
  * state. If this function is successful the function deallocate_next_states()
  * should be called when the lexical analisys is done.
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
static LAH_Syntax_State_Transition* allocate_next_states_once = NULL;
static LAH_Syntax_State_Transition* get_or_create_next_states(void)
{
	if (allocate_next_states_once)
	{
		return allocate_next_states_once;
	}

	allocate_next_states_once = calloc(((size_t)LAH_ERROR_STATE) + 1, sizeof(*allocate_next_states_once));
	if (!allocate_next_states_once)
	{
		ERH_report_error_generic("In create_next_states(), memory allocation for next_states failed\n");
		return allocate_next_states_once;
	}

	allocate_next_states_once[LAH_START_STATE] = (LAH_Syntax_State_Transition){ LAH_START_STATE, {LAH_ENTER_OPCODE_STATE, LAH_ERROR_STATE,
		LAH_ENTER_OPERAND_STATE, LAH_OPCODE_STATE, LAH_OPERAND_STATE, LAH_START_STATE, LAH_DONE_STATE, LAH_ERROR_STATE} };
	allocate_next_states_once[LAH_ENTER_OPCODE_STATE] = (LAH_Syntax_State_Transition){ LAH_ENTER_OPCODE_STATE, {LAH_ENTER_OPCODE_STATE,
		LAH_END_STATEMENT_STATE, LAH_ENTER_OPERAND_STATE, LAH_OPCODE_STATE, LAH_OPERAND_STATE, LAH_ENTER_OPCODE_STATE,
		LAH_ERROR_STATE, LAH_ERROR_STATE} };
	allocate_next_states_once[LAH_OPCODE_STATE] = (LAH_Syntax_State_Transition){ LAH_OPCODE_STATE, {LAH_ERROR_STATE, LAH_END_STATEMENT_STATE,
		LAH_ENTER_OPERAND_STATE, LAH_OPCODE_STATE, LAH_OPERAND_STATE, LAH_END_OPCODE_STATE, LAH_ERROR_STATE, LAH_ERROR_STATE} };
	allocate_next_states_once[LAH_END_OPCODE_STATE] = (LAH_Syntax_State_Transition){ LAH_END_OPCODE_STATE, {LAH_ERROR_STATE,
		LAH_END_STATEMENT_STATE, LAH_ENTER_OPERAND_STATE, LAH_ERROR_STATE, LAH_OPERAND_STATE, LAH_END_OPCODE_STATE,
		LAH_ERROR_STATE, LAH_ERROR_STATE} };
	allocate_next_states_once[LAH_ENTER_OPERAND_STATE] = (LAH_Syntax_State_Transition){ LAH_ENTER_OPERAND_STATE, {LAH_ERROR_STATE,
		LAH_END_STATEMENT_STATE, LAH_DONE_STATE, LAH_ERROR_STATE, LAH_OPERAND_STATE, LAH_ENTER_OPERAND_STATE, LAH_ERROR_STATE} };
	allocate_next_states_once[LAH_OPERAND_STATE] = (LAH_Syntax_State_Transition){ LAH_OPERAND_STATE, {LAH_ERROR_STATE, LAH_END_STATEMENT_STATE,
		LAH_DONE_STATE, LAH_ERROR_STATE, LAH_OPERAND_STATE, LAH_END_OPERAND_STATE, LAH_ERROR_STATE, LAH_ERROR_STATE} };
	allocate_next_states_once[LAH_END_OPERAND_STATE] = (LAH_Syntax_State_Transition){ LAH_END_OPERAND_STATE, {LAH_ERROR_STATE,
		LAH_END_STATEMENT_STATE, LAH_DONE_STATE, LAH_ERROR_STATE, LAH_ERROR_STATE, LAH_END_OPERAND_STATE, LAH_ERROR_STATE, LAH_ERROR_STATE} };
	allocate_next_states_once[LAH_END_STATEMENT_STATE] = (LAH_Syntax_State_Transition){ LAH_END_STATEMENT_STATE, {LAH_ERROR_STATE,
		LAH_END_STATEMENT_STATE, LAH_DONE_STATE, LAH_ERROR_STATE, LAH_ERROR_STATE, LAH_END_STATEMENT_STATE, LAH_DONE_STATE, LAH_ERROR_STATE} };
	allocate_next_states_once[LAH_DONE_STATE] = (LAH_Syntax_State_Transition){ LAH_DONE_STATE, {LAH_ERROR_STATE, LAH_ERROR_STATE,
		LAH_DONE_STATE, LAH_ERROR_STATE, LAH_ERROR_STATE, LAH_DONE_STATE, LAH_DONE_STATE, LAH_ERROR_STATE} };
	allocate_next_states_once[LAH_ERROR_STATE] = (LAH_Syntax_State_Transition){ LAH_ERROR_STATE, {LAH_ERROR_STATE, LAH_ERROR_STATE,
		LAH_ERROR_STATE, LAH_ERROR_STATE, LAH_ERROR_STATE, LAH_ERROR_STATE, LAH_ERROR_STATE, LAH_ERROR_STATE} };

	return allocate_next_states_once;
}

/*
 * Called when the lexical analysis is completed, to delete any memory
 * allocated by the lexical analyzer. To ensure that the lexical analyzer
 * can run more than once make sure the pointer to the memory is set to NULL.
 */
void deactivate_lexical_analyzer(void)
{
	free(allocate_next_states_once);
	allocate_next_states_once = NULL;
}

/*
 * A design decision was made to allocate next_states only once to save overhead in
 * this function and to not force the parser to allocate the memory.
 *
 * This function performs the lexical analysis for the parser, it uses a state machine
 * implemented as a table to do this. That table is the next_states variable.
 */
LAH_Syntax_State lexical_analyzer(const LAH_Syntax_State current_state, Const_U_Char input, unsigned syntax_check_list[])
{
	LAH_Syntax_State_Transition* next_states = get_or_create_next_states();
	if (!next_states)
	{
		fprintf(ERH_error_out_file, "In %s: Memory allocation error in get_or_create_next_states()\n", "get_state_transition_collect_parser_error_data");
		fprintf(ERH_error_out_file, "Unable to perform lexical analisys! Exiting program.");
		exit(EXIT_FAILURE);
	}

	LAH_State_Transition_Characters character_type = get_transition_character_type(input, current_state);
	collect_error_reporting_data(current_state, character_type, syntax_check_list);

	return next_states[current_state].transition_on_char_type[character_type];
}
#endif	// INCLUDED_IN_UNIT_TEST

#endif	// LEXICAL_ANALYZER_C
