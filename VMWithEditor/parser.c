/* parser.c
 *
 * The functions in this file are responsible for parsing any files input by
 * the user that contain virtual memory programs.
 */
#ifndef PARSER_C
#define PARSER_C

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "vmwitheditor.h"
#include "ERH_error_reporting.h"
#include "HRF_human_readable_program_format.h"
#include "LAH_lexical_analyzer.h"
#include "SSF_safe_string_functions.h"
#include "parser.h"
#include "VMH_virtual_machine.h"

 /*
 * Syntax checking starts here
 */
static void init_error_strings(const char* error_strings[LAH_SYNTAX_CHECK_ARRAY_SIZE])
{
	error_strings[LAH_OPENBRACE] = "Missing the opening brace.";
	error_strings[LAH_CLOSEBRACE] = "Missing the closing brace.";
	error_strings[LAH_COMMA] = "Missing comma(s)";
	error_strings[LAH_LEGALOPCODE] = "Missing or unknow opcode";
	error_strings[LAH_LEGALOPERAND] = "Missing operand or operand out of range";
	error_strings[LAH_ILLEGALOPCODE] = "Unknown Opcode.";
	error_strings[LAH_ILLEGALFIRSTCHAR] =
		"Illegal character in column 1 (are you missing the opening brace { )";
	error_strings[LAH_MULTIPLESTATEMENTSONELINE] = "Only one program step per line";
	error_strings[LAH_MISSINGCOMMA] = "Missing comma(s)";
	error_strings[LAH_ILLEGALCHAR] = "Illegal character";
}

static bool print_syntax_errors(const unsigned* necessary_items, size_t* line_number,
	const char* file_name, Const_U_Char* line)
{
	bool syntax_is_good = true;
	unsigned error_count = 0;

	char* error_strings[LAH_SYNTAX_CHECK_ARRAY_SIZE];
	init_error_strings(error_strings);
	char illegal_operand[256];
	snprintf(illegal_operand, sizeof(illegal_operand),
		"The operand is out of range [0x0, 0x%zx]",
		VMH_get_maximum_operand_value());
	error_strings[LAH_ILLEGALOPERAND] = illegal_operand;

	for (size_t i = 0; i < LAH_SYNTAX_CHECK_ARRAY_SIZE; i++)
	{
		error_count += (!necessary_items[i] && i < LAH_ILLEGALOPCODE) ? 1 :
			((i >= LAH_ILLEGALOPCODE && necessary_items[i]) ? 1 : 0);
	}

	if (error_count)
	{
		ERH_va_report_error_fprintf("In file %s line %zu there are %u errors: %s\n",
			file_name, *line_number, error_count, line);
		ERH_va_report_error_fprintf(
			"    The proper format of a program step is { OPCODE , OPERAND }[,]"
			" the second comma is necessary when the program step is not the "
			"last one in the file. ");
		ERH_va_report_error_fprintf("Only one program step per line\n");
		syntax_is_good = false;
		if (!necessary_items[LAH_OPENBRACE])		// Prevent cascading error messages that may not be correct
		{
			ERH_va_report_error_fprintf("\t%s\n\n", error_strings[LAH_OPENBRACE]);
			return syntax_is_good;
		}
	}

	for (size_t i = 0; i < LAH_SYNTAX_CHECK_ARRAY_SIZE; i++)
	{
		if (i >= LAH_ILLEGALOPCODE && necessary_items[i])
		{
			ERH_va_report_error_fprintf("\t%s\n", error_strings[i]);
		}
		else if (i < LAH_ILLEGALOPCODE && !necessary_items[i])
		{
			ERH_va_report_error_fprintf("\t%s\n", error_strings[i]);
		}
	}

	ERH_va_report_error_fprintf("\n");

	return syntax_is_good;
}

static void get_opcode_or_operand_string(Const_U_Char* original, size_t length, unsigned char* possible_op)
{
	char* functtion_name = "get_opcode_or_operand_string";
	if (original && length)
	{
		strncpy((char*)possible_op, (char*)original, length);
	}
	else
	{
		char pointer_numeric_value[ERH_ERROR_BUFFER_SIZE];
		snprintf(pointer_numeric_value, sizeof(pointer_numeric_value),
			" original = 0x%" PRIXPTR, (uintptr_t)original);
		ERH_va_report_error_fprintf(
			"Internal error: %s: illegal value opcode or operand string %s lenght = %zu\n",
			functtion_name, pointer_numeric_value, length);
	}
}

/*
 * Returns the value of the opcode or operand is valid, -1 otherwise. Updates
 * the syntax item check list.
 */
static int validate_opcode_or_operand(Const_U_Char *possible_op,
	const LAH_Syntax_State state, unsigned syntax_check_list[])
{
	int test_value = -1;
	LAH_Syntax_Check_List_Items legal_index = LAH_OPENBRACE;
	LAH_Syntax_Check_List_Items illegal_index = LAH_OPENBRACE;

	if (state == LAH_OPCODE_STATE)
	{
		test_value = OPC_translate_string_to_opcode(possible_op);
		legal_index = LAH_LEGALOPCODE;
		illegal_index = LAH_ILLEGALOPCODE;
	}
	else if (state == LAH_OPERAND_STATE)
	{
		test_value = VMH_translate_text_to_operand_and_validate((char*)possible_op);
		legal_index = LAH_LEGALOPERAND;
		illegal_index = LAH_ILLEGALOPERAND;
	}
	else
	{
		return test_value;
	}

	if (test_value < 0)
	{
		syntax_check_list[legal_index] = 0;
		syntax_check_list[illegal_index]++;
	}
	else
	{
		syntax_check_list[legal_index]++;
	}

	return test_value;
}

#define MAX_OP_LENGTH 64
/* 
 * Returns an opcode or operand if the input string value is legal, (unsigned) -1
 * otherwise. Updates the syntax item check list as necessary.
 */
static unsigned int get_legal_opcode_or_oparand(Const_U_Char* current_character,
	const size_t possible_op_length, unsigned syntax_check_list[],
	const LAH_Syntax_State state)
{
	unsigned int return_val = (unsigned int)-1;
	if (state != LAH_OPCODE_STATE && state != LAH_OPERAND_STATE)
	{
		ERH_report_error_generic(
			"\nInternal error: get_legal_opcode_or_oparand() : Illegal state.\n");
	}
	else
	{
		if (possible_op_length < MAX_OP_LENGTH)
		{
			unsigned char possible_op[MAX_OP_LENGTH];
			memset(possible_op, 0, sizeof(possible_op));
			get_opcode_or_operand_string(current_character, possible_op_length,
				possible_op);
			if (strlen((char *)possible_op))
			{
				return_val = validate_opcode_or_operand(possible_op, state,
					syntax_check_list);
			}
		}
		else
		{
			ERH_va_report_error_fprintf(
				"Internal error: %s : string length %zu greater than buffer size %d\n",
				"get_legal_opcode_or_oparand", possible_op_length, MAX_OP_LENGTH);
		}
	}

	return return_val;
}

static void check_for_required_character(Const_U_Char current_character,
	const LAH_Syntax_Check_List_Items item_to_check, const unsigned max_repetition,
	unsigned syntax_check_list[])
{
	unsigned char test_comparitor = 0;
	switch (item_to_check)
	{
	case LAH_OPENBRACE:
		test_comparitor = '{';
		break;
	case LAH_COMMA:
		test_comparitor = ',';
		break;
	case LAH_CLOSEBRACE:
		test_comparitor = '}';
		break;
	}

	if (current_character == test_comparitor)
	{
		syntax_check_list[item_to_check]++;
		if (syntax_check_list[item_to_check] > max_repetition)
		{
			syntax_check_list[LAH_MULTIPLESTATEMENTSONELINE]++;
		}
	}
}

/*
 * Exclude the following public interfaces from being included in the unit test
 * files to prevent linker errors.
 */
#ifndef INTERNAL_PARSER_TESTS_C
/*
 * Called when the parser is completed, to delete any memory allocated by the
 * parser. The progam steps returned by the parser are owned by the calling 
 * function and will be freed by the calling function when necessary.
 */
void deactivate_parser(void)
{
	deactivate_lexical_analyzer();
}

/*
 * Expected syntax:		{opcode, operand} with possible comma at the end.
 */
HRF_Program_Step_Node* parser(Const_U_Char* text_line, size_t* line_number, const char* file_name)
{
	unsigned syntax_check_list[LAH_SYNTAX_CHECK_ARRAY_SIZE];
	memset(&syntax_check_list[0], 0, sizeof(syntax_check_list));
	HRF_Human_Readable_Program_Format legal = { OPC_OPCODE_TRANSLATOR_ARRAY_SIZE, 0 };
	LAH_Syntax_State current_state = LAH_START_STATE;
	size_t possible_op_char_count = 0;
	Const_U_Char* possible_op = NULL;

	Const_U_Char* current_character = text_line;
	while (*current_character)
	{
		LAH_Syntax_State new_state = lexical_analyzer(current_state, *current_character, syntax_check_list);
		if (new_state != current_state)
		{
			if (new_state == LAH_OPCODE_STATE)
			{
				possible_op = current_character;
				possible_op_char_count = 1;
			}

			if (new_state == LAH_OPERAND_STATE)
			{
				possible_op = current_character;
				possible_op_char_count = 1;
			}
			//TODO Correct state transition testing
			if (current_state == LAH_OPCODE_STATE)
			{
   				legal.opcode = get_legal_opcode_or_oparand(possible_op, possible_op_char_count, syntax_check_list, current_state);
				possible_op_char_count = 0;
			}

			if (current_state == LAH_OPERAND_STATE)
			{
				if (!syntax_check_list[LAH_COMMA])
				{
					syntax_check_list[LAH_MISSINGCOMMA]++;
				}
				legal.operand = get_legal_opcode_or_oparand(possible_op, possible_op_char_count, syntax_check_list, current_state);
				possible_op_char_count = 0;
			}
			current_state = new_state;
		}
		else
		{
			if (new_state == LAH_OPCODE_STATE || new_state == LAH_OPERAND_STATE)
			{
				possible_op_char_count++;
			}
		}

		current_character++;
	}

	HRF_Program_Step_Node* next_node = NULL;
	if (print_syntax_errors(syntax_check_list, line_number, file_name, text_line))
	{
		next_node = HRF_create_program_step(&legal);
		(*line_number)++;
	}

	return next_node;
}

#endif

#endif // !PARSER_C

