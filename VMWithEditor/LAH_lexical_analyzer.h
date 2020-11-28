/*
 * LAH_lexical_analyzer.h
 *
 * The Syntax State Machine is a simple lexical analiser. Given the current syntax
 * state and the new input character what is the new syntax state. State machines
 * can be represented as tables. Table implementation of a state machine uses
 * more memory but performs faster, the lexical analyser programs Flex and LEX
 * generate tables to implement lexical analysis.
 *
 * This module uses enums to make the states and transitions easier to understand.
 * All Macros and enums are prefaced with LAH_ to indicate they come from
 * LAH_lexical_analyzer.h.
 *
 */
#ifndef LAH_LEXICAL_ANALYZER_H
#define LAH_LEXICAL_ANALYZER_H

#include "vmwitheditor.h"

typedef enum syntax_check_list_items
{
	LAH_OPENBRACE = 0,
	LAH_CLOSEBRACE = 1,
	LAH_COMMA = 2,
	LAH_LEGALOPCODE = 3,
	LAH_LEGALOPERAND = 4,
	LAH_ILLEGALOPCODE = 5,
	LAH_ILLEGALOPERAND = 6,
	LAH_ILLEGALFIRSTCHAR = 7,
	LAH_MULTIPLESTATEMENTSONELINE = 8,
	LAH_ILLEGALCHAR = 9,
	LAH_MISSINGCOMMA = 10
#define LAH_SYNTAX_CHECK_ARRAY_SIZE (10 + 1)
} LAH_Syntax_Check_List_Items;

typedef enum syntax_state_enum
{
	LAH_START_STATE = 0,				// Start of a new line, only white space or open brace is really expected
	LAH_ENTER_OPCODE_STATE = 1,			// Open brace encountered, waiting for opcode (first alpha character) white space or alpha is expected
	LAH_OPCODE_STATE = 2,				// Open brace and first leter of opcode have been encoutered more alpha, white space or comma expected
	LAH_END_OPCODE_STATE = 3,			// White space has been encountered only white space or comma expected
	LAH_ENTER_OPERAND_STATE = 4,		// Comma has been encountered, waiting for first digit of operand white space allowed
	LAH_OPERAND_STATE = 5,				// First digit of operand has been encountered, remain in this state until white space or close brace is encountered.
	LAH_END_OPERAND_STATE = 6,			// White space has been encountered, waiting for close brace to end statement
	LAH_END_STATEMENT_STATE = 7,		// Close brace has been encountered, comma or new line expected
	LAH_DONE_STATE = 8,					// Comma has been encountered only legal input is white space or new line
	LAH_ERROR_STATE = 9
} LAH_Syntax_State;

#define LAH_SYNTAX_STATE_ARRAY_SIZE	(9 + 1)	// (size_t) ERROR_STATE + 1

typedef enum legal_characters_that_cause_transitions
{
	LAH_OPENBRACE_STATE_TRANSITION = 0,			// This needs to be the same as OPENBRACE in Syntax_Check_List_Items
	LAH_CLOSEBRACE_STATE_TRANSITION = 1,		// This needs to be the same as CLOSEBRACE in Syntax_Check_List_Items
	LAH_COMMA_STATE_TRANSITION = 2,				// This needs to be the same as COMMA in Syntax_Check_List_Items
	LAH_ALPHA_STATE_TRANSITION = 3,
	LAH_DIGIT_STATE_TRANSITION = 4,
	LAH_WHITESPACE_STATE_TRANSITION = 5,
	LAH_EOL_STATE_TRANSITION = 6,				// End of Line
	LAH_ILLEGAL_CHAR_TRANSITION = 7
} LAH_State_Transition_Characters;
#define LAH_TRANSITION_ARRAY_SIZE (7 + 1)		// ILLEGAL_CHAR_TRANSITION + 1

typedef struct syntax_state_transition
{
	LAH_Syntax_State current_state;
	LAH_Syntax_State transition_on_char_type[LAH_TRANSITION_ARRAY_SIZE];
} LAH_Syntax_State_Transition;

#define LAH_MAX_COMMA 2
#define LAH_MAX_OPEN_BRACE 1
#define LAH_MAX_CLOSE_BRACE 1
#define LAH_MAX_OPCODE 1
#define LAH_MAX_OPERAND 1
#define LAH_MAX_WHITE_SPACE	200

LAH_Syntax_State lexical_analyzer(const LAH_Syntax_State current_state,
	Const_U_Char input, unsigned syntax_check_list[]);
void deactivate_lexical_analyzer(void);

#endif	/*	!LAH_LEXICAL_ANALYZER_H */
