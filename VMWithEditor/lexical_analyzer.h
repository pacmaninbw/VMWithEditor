/*
 * lexical_analyzer.h
 *
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
#ifndef SYNTAX_STATE_MACHINE_H
#define SYNTAX_STATE_MACHINE_H

typedef enum syntax_checks_list_items
{
	OPENBRACE = 0,
	CLOSEBRACE = 1,
	COMMA = 2,
	LEGALOPCODE = 3,
	LEGALOPERAND = 4,
	ILLEGALOPCODE = 5,
	ILLEGALOPERAND = 6,
	ILLEGALFIRSTCHAR = 7,
	MULTIPLESTATEMENTSONELINE = 8,
	ILLEGALCHAR = 9,
	MISSINGCOMMA = 10
#define SYNTAX_CHECK_COUNT 11
} Syntax_Check_List_Items;

typedef enum syntax_state_enum
{
	START_STATE = 0,				// Start of a new line, only white space or open brace is really expected
	ENTER_OPCODE_STATE = 1,			// Open brace encountered, waiting for opcode (first alpha character) white space or alpha is expected
	OPCODE_STATE = 2,				// Open brace and first leter of opcode have been encoutered more alpha, white space or comma expected
	END_OPCODE_STATE = 3,			// White space has been encountered only white space or comma expected
	ENTER_OPERAND_STATE = 4,		// Comma has been encountered, waiting for first digit of operand white space allowed
	OPERAND_STATE = 5,				// First digit of operand has been encountered, remain in this state until white space or close brace is encountered.
	END_OPERAND_STATE = 6,			// White space has been encountered, waiting for close brace to end statement
	END_STATEMENT_STATE = 7,		// Close brace has been encountered, comma or new line expected
	DONE_STATE = 8,					// Comma has been encountered only legal input is white space or new line
	ERROR_STATE = 9
} Syntax_State;

#define SYNTAX_STATE_ARRAY_SIZE	9 + 1	// (size_t) ERROR_STATE + 1

typedef enum legal_characters_that_cause_transitions
{
	OPENBRACE_STATE_TRANSITION = 0,			// This needs to be the same as OPENBRACE in Syntax_Check_List_Items
	CLOSEBRACE_STATE_TRANSITION = 1,		// This needs to be the same as CLOSEBRACE in Syntax_Check_List_Items
	COMMA_STATE_TRANSITION = 2,				// This needs to be the same as COMMA in Syntax_Check_List_Items
	ALPHA_STATE_TRANSITION = 3,
	DIGIT_STATE_TRANSITION = 4,
	WHITESPACE_STATE_TRANSITION = 5,
	EOL_STATE_TRANSITION = 6,		// End of Line
	ILLEGAL_CHAR_TRANSITION = 7
} State_Transition_Characters;
#define TRANSITION_ARRAY_SIZE 7 + 1		// ILLEGAL_CHAR_TRANSITION + 1

typedef struct syntax_state_transition
{
	Syntax_State current_state;
	Syntax_State transition_on_char_type[TRANSITION_ARRAY_SIZE];
} Syntax_State_Transition;

#define MAX_COMMA 2
#define MAX_OPEN_BRACE 1
#define MAX_CLOSE_BRACE 1
#define MAX_OPCODE 1
#define MAX_OPERAND 1
#define MAX_WHITE_SPACE	200

extern Syntax_State lexical_analyzer(Syntax_State current_state, unsigned char input, unsigned syntax_check_list[]);
extern void deactivate_lexical_analyzer(void);

#endif	//	SYNTAX_STATE_MACHINE_H
