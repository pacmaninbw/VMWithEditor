/* parser.c
 *
 * The functions in this file are responsible for parsing any files input by
 * the user that contain virtual memory programs.
 */
#ifndef PARSER_C
#define PARSER_C

#include <errno.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vmwitheditor.h"
#include "ERH_error_reporting.h"
#include "HRF_human_readable_program_format.h"
#include "LAH_lexical_analyzer.h"
#include "SSF_safe_string_functions.h"
#include "parser.h"
#include "VMH_virtual_machine.h"
#ifdef UNIT_TESTING
#include "UTL_unit_test_logging.h"
#endif
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
	error_strings[LAH_ILLEGALFIRSTCHAR] = "Illegal character in column 1 (are you missing the opening brace { )";
	error_strings[LAH_MULTIPLESTATEMENTSONELINE] = "Only one program step per line";
}

static bool print_syntax_errors(const unsigned* necessary_items, size_t* line_number,
	const char* file_name, Const_U_Char* line)
{
	bool syntax_is_good = true;
	unsigned error_count = 0;

	char* error_strings[LAH_SYNTAX_CHECK_ARRAY_SIZE];
	init_error_strings(error_strings);
	char illegal_operand[256];
	snprintf(illegal_operand, sizeof(illegal_operand), "The operand is out of range [0x0, 0x%zx]", VMH_get_maximum_operand_value());
	error_strings[LAH_ILLEGALOPERAND] = illegal_operand;

	for (size_t i = 0; i < LAH_SYNTAX_CHECK_ARRAY_SIZE; i++)
	{
		error_count += (!necessary_items[i] && i < LAH_ILLEGALOPCODE) ? 1 : ((i >= LAH_ILLEGALOPCODE && necessary_items[i]) ? 1 : 0);
	}

	if (error_count)
	{
		ERH_va_report_error_fprintf("In file %s line %zu there are %u errors: %s\n",
			file_name, *line_number, error_count, line);
		ERH_va_report_error_fprintf("    The proper format of a program step is { OPCODE , OPERAND }[,]"
			" the second comma is necessary when the program step is not the last one in the file. ");
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

static unsigned char* get_opcode_or_operand_string(Const_U_Char* original)
{
	unsigned char* local_copy = SSF_ucstrdup(original);
	if (!local_copy)
	{
		return local_copy;
	}
	register unsigned char* current_char = local_copy;
	unsigned char* return_string = NULL;

	while (isalnum(*current_char))
	{
		current_char++;
	}

	*current_char = '\0';

	return_string = SSF_ucstrdup(local_copy);
	free(local_copy);

	return return_string;
}

static unsigned get_legal_opcode_or_oparand(Const_U_Char** current_character,
	unsigned syntax_check_list[], const LAH_Syntax_Check_List_Items prime_index)
{
	if (prime_index != LAH_LEGALOPCODE && prime_index != LAH_LEGALOPERAND)
	{
		ERH_va_report_error_fprintf(
			"\n\n*** Illegal prime_index in get_legal_opcode_or_oparand()\n\n ***");
		return (unsigned)-1;
	}

	unsigned char* possible_op = get_opcode_or_operand_string(*current_character);
	if (!possible_op)
	{
		char *err_msg = "get_opcode_or_operand_string: Failed: returned NULL";
		if (errno)
		{
			perror(err_msg);
		}
		else
		{
			ERH_va_report_error_fprintf("%s\n", err_msg);
		}
		return (unsigned)-1;
	}

	int test_value = (prime_index == LAH_LEGALOPCODE) ? OPC_translate_string_to_opcode(possible_op) :
		VMH_translate_text_to_operand_and_validate((char*)possible_op);
	int minimum_value = (prime_index == LAH_LEGALOPCODE) ? (int)OPC_HALT : 0;

	if (test_value < minimum_value)
	{
		syntax_check_list[prime_index] = 0;
		syntax_check_list[(prime_index == LAH_LEGALOPCODE) ? LAH_ILLEGALOPCODE :
			LAH_ILLEGALOPERAND]++;
	}
	else
	{
		syntax_check_list[prime_index]++;
	}

	*current_character += (strlen((char*)possible_op) - 1);		// advance the current character past the string processed.
	free(possible_op);											// No memory leaks allocated in get_opcode_or_operand_string()

	return (unsigned)minimum_value;
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

/* Expected syntax:		{opcode, operand} with possible comma at the end.
 *
 * To prevent the global function parser from being defined in multiple object
 * file when this file is included for unit test we check to see if we are in
 * the internal parser unit test file.
 */
#ifndef INTERNAL_PARSER_TESTS_C
HRF_Program_Step_Node* parser(Const_U_Char* text_line, size_t* line_number, const char* file_name)
{
	unsigned syntax_check_list[LAH_SYNTAX_CHECK_ARRAY_SIZE];
	memset(&syntax_check_list[0], 0, sizeof(syntax_check_list));
	HRF_Human_Readable_Program_Format legal = { OPC_OPCODE_TRANSLATOR_ARRAY_SIZE, 0 };
	LAH_Syntax_State current_state = LAH_START_STATE;

	Const_U_Char* current_character = text_line;
	while (*current_character)
	{
		LAH_Syntax_State new_state = lexical_analyzer(current_state, *current_character, syntax_check_list);
		if (new_state != current_state)
		{
			//TODO Correct state transition testing
			if (current_state == LAH_ENTER_OPCODE_STATE && isalpha(*current_character))
			{
				legal.opcode = get_legal_opcode_or_oparand(&current_character, syntax_check_list, LAH_LEGALOPCODE);
			}

			if (syntax_check_list[(size_t)LAH_OPENBRACE] && syntax_check_list[(size_t)LAH_COMMA] && isalnum(*current_character))
			{
				legal.operand = get_legal_opcode_or_oparand(&current_character, syntax_check_list, LAH_LEGALOPERAND);
			}
			current_state = new_state;
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

