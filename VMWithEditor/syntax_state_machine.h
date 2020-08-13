#ifndef SYNTAX_STATE_MACHINE_H
#define SYNTAX_STATE_MACHINE_H

typedef enum syntax_checks_offss
{
	OPENBRACE = 0,
	CLOSEBRACE = 1,
	COMMA = 2,
	LEGALOPCODE = 3,
	LEGALOPERAND = 4,
	ILLEGALOPCODE = 5,
	ILLEGALOPERAND = 6,
	ILLEGALFIRSTCHAR = 7,
	MULTIPLESTATEMENTSONELINE = 8
#define SYNTAX_CHECK_COUNT 9
} Syntax_Check_Offss;

typedef enum Syntax_State
{
	START_STATE = 0,
	ENTER_OPCODE_STATE,
	OPCODE_STATE,
	END_OPCODE_STATE,
	ENTER_OPERAND_STATE,
	OPERAND_STATE,
	END_OPERAND_STATE,
	END_STATEMENT_STATE,
	DONE_STATE,
	ERROR_STATE = 9
} Syntax_State;

typedef struct syntax_state_transition
{
	Syntax_State legal_next_state;
	Syntax_State error_with_next_state;
} Syntax_State_Transition;

#define MAX_COMMA 2
#define MAX_OPEN_BRACE 1
#define MAX_CLOSE_BRACE 1

extern Syntax_State state_transition(Syntax_State current_state, unsigned char* input, unsigned syntax_check_list[]);

#endif	//	SYNTAX_STATE_MACHINE_H
