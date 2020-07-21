#include "human_readable_program_format.h"
#include "virtual_machine.h"
#include "common_program_logic.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef UNIT_TESTING
static FILE* unit_test_log_file = NULL;
#endif

Human_Readable_Program_Format* duplicate_program(Human_Readable_Program_Format* program, size_t program_size)
{
	Human_Readable_Program_Format* copy_of_program = calloc(sizeof(*copy_of_program), program_size);
	if (copy_of_program)
	{
		register Human_Readable_Program_Format* copy_current_ptr = copy_of_program;
		register Human_Readable_Program_Format* prog_current_ptr = program;

		// tried memcpy, it doesn't work properly.
		for (size_t i = 0; i < program_size; i++, copy_current_ptr++, prog_current_ptr++)
		{
			*copy_current_ptr = *prog_current_ptr;
		}

	}
	else
	{
		fprintf(error_out_file, "Memory allocation for copy of program failed.\n");
	}

	return copy_of_program;
}

Program_Step_Node* create_program_step(Human_Readable_Program_Format *user_input)
{
	Program_Step_Node* program_step = calloc(1, sizeof(*program_step));
	if (!program_step)
	{
		fprintf(error_out_file, "In create_program_step(), memory allocation for next_step failed\n");
	}
	else
	{
		program_step->opcode_and_operand.opcode = user_input->opcode;
		program_step->opcode_and_operand.operand = user_input->operand;
		// program_step->next_step defaults to NULL due to calloc().
	}

	return program_step;
}

Human_Readable_Program_Format* convert_link_list_program_to_array(Program_Step_Node *linked_program, size_t program_size)
{
	Human_Readable_Program_Format* array_program = calloc(program_size, sizeof(*array_program));
	if (!array_program)
	{
		fprintf(error_out_file, "In convert_link_list_program_to_array(), memory allocation for array_program failed\n");
	}
	else
	{
		register Program_Step_Node* source = linked_program;
		register Human_Readable_Program_Format* destination = array_program;

		while (source->next_step)
		{
			*destination = source->opcode_and_operand;
			source = source->next_step;
			destination++;
		}
	}

	return array_program;
}

/* 
 * Syntax checking starts here
 */
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

static void init_error_strings(char * error_strings[SYNTAX_CHECK_COUNT])
{
	error_strings[OPENBRACE] = "Missing the opening brace.";
	error_strings[CLOSEBRACE] = "Missing the closing brace.";
	error_strings[COMMA] = "Missing comma(s)";
	error_strings[LEGALOPCODE] = "Missing or unknow opcode";
	error_strings[LEGALOPERAND] = "Missing operand or operand out of range";
	error_strings[ILLEGALOPCODE] = "Unknown Opcode.";
	error_strings[ILLEGALFIRSTCHAR] = "Illegal character in column 1 (are you missing the opening brace { )";
	error_strings[MULTIPLESTATEMENTSONELINE] = "Only one program step per line";
}

static const unsigned MAX_COMMA = 2;
static const unsigned MAX_OPEN_BRACE = 1;
static const unsigned MAX_CLOSE_BRACE = 1;

static bool print_syntax_errors(unsigned *necessary_items, size_t *line_number, char* file_name, unsigned char *line)
{
	bool syntax_is_good = true;
	unsigned error_count = 0;

	char* error_strings[SYNTAX_CHECK_COUNT];
	init_error_strings(error_strings);
	char illegal_operand[256];
	sprintf(illegal_operand, "The operand is out of range [0x0, 0x%zx]", get_maximum_operand_value());
	error_strings[ILLEGALOPERAND] = illegal_operand;

	for (size_t i = 0; i < SYNTAX_CHECK_COUNT; i++)
	{
		error_count += (!necessary_items[i] && i < ILLEGALOPCODE) ? 1 : ((i >= ILLEGALOPCODE && necessary_items[i]) ? 1 : 0);
	}

	if (error_count)
	{
		fprintf(error_out_file, "In file %s line %d there are %d errors: %s\n", file_name, (int)*line_number, error_count, line);
		fprintf(error_out_file, "    The proper format of a program step is { OPCODE , OPERAND }[,] the second comma is necessary when the program step is not the last one in the file. ");
		fprintf(error_out_file, "Only one program step per line\n");
		syntax_is_good = false;
		if (!necessary_items[OPENBRACE])		// Prevent cascading error messages that may not be correct
		{
			fprintf(error_out_file, "\t%s\n\n", error_strings[OPENBRACE]);
			fflush(error_out_file);
			return syntax_is_good;
		}
	}

	for (size_t i = 0; i < SYNTAX_CHECK_COUNT; i++)
	{
		if (i >= ILLEGALOPCODE && necessary_items[i])
		{
			fprintf(error_out_file, "\t%s\n", error_strings[i]);
		}
		else if (i < ILLEGALOPCODE && !necessary_items[i])
		{
			fprintf(error_out_file, "\t%s\n", error_strings[i]);
		}
	}

	fprintf(error_out_file, "\n");
	fflush(error_out_file);

	return syntax_is_good;
}

static unsigned char* get_opcode_or_operand_string(char *original)
{
	char *local_copy = _strdup(original);
	if (!local_copy)
	{
		return (unsigned char*) local_copy;
	}
	register unsigned char *current_char = (unsigned char *) local_copy;
	unsigned char *return_string = NULL;

	while (isalnum(*current_char))
	{
		current_char++;
	}

	*current_char = '\0';

	return_string = (unsigned char *) _strdup((char *)local_copy);
	free(local_copy);

	return return_string;
}

static unsigned get_legal_opcode_or_oparand(unsigned char** current_character, unsigned syntax_check_list[], Syntax_Check_Offss prime_index)
{
	if (prime_index != LEGALOPCODE && prime_index != LEGALOPERAND)
	{
		fprintf(error_out_file, "\n\n*** Illegal prime_index in get_legal_opcode_or_oparand()\n\n ***");
		return (unsigned)-1;
	}

	unsigned char* possible_op = get_opcode_or_operand_string((char *)*current_character);

	int test_value = (prime_index == LEGALOPCODE) ? translate_string_to_opcode(possible_op) : translate_text_to_operand_and_validate((char*)possible_op);
	int minimum_value = (prime_index == LEGALOPCODE) ? (int)HALT : 0;

	if (test_value < minimum_value)
	{
		syntax_check_list[prime_index] = 0;
		syntax_check_list[(prime_index == LEGALOPCODE) ? ILLEGALOPCODE : ILLEGALOPERAND]++;
	}
	else
	{
		syntax_check_list[prime_index]++;
	}

	*current_character += (strlen((char *) possible_op) -1);		// advance the current character past the string processed.
	free(possible_op);												// No memory leaks allocated in get_opcode_or_operand_string()

	return (unsigned) minimum_value;
}

static void check_for_required_character(unsigned char current_character, Syntax_Check_Offss item_to_check, unsigned max_repetition, unsigned syntax_check_list[])
{
	unsigned char test_comparitor = 0;
	switch (item_to_check)
	{
	case OPENBRACE:
		test_comparitor = '{';
		break;
	case COMMA:
		test_comparitor = ',';
		break;
	case CLOSEBRACE:
		test_comparitor = '}';
		break;
	}

	if (current_character == test_comparitor)
	{
		syntax_check_list[item_to_check]++;
		if (syntax_check_list[item_to_check] > max_repetition)
		{
			syntax_check_list[MULTIPLESTATEMENTSONELINE]++;
		}
	}

}

/* Expected syntax
 * {opcode, operand} with possible comma at the end.
 */
Program_Step_Node* hrf_check_line_syntax_return_program_step_if_valid(unsigned char* text_line, size_t *line_number, char *file_name)
{
	unsigned syntax_check_list[SYNTAX_CHECK_COUNT];
	memset(&syntax_check_list[0], 0, sizeof(syntax_check_list));
	Human_Readable_Program_Format legal;
	legal.opcode = (OPCODE)OPCODE_TRANSLATOR_COUNT;
	legal.operand = 0;

	if (!isspace(*text_line) && *text_line != '{')
	{
		syntax_check_list[ILLEGALFIRSTCHAR]++;
	}

	unsigned char* current_character = text_line;
	while (*current_character)
	{
		check_for_required_character(*current_character, OPENBRACE, MAX_OPEN_BRACE, syntax_check_list);

		check_for_required_character(*current_character, COMMA, MAX_COMMA, syntax_check_list);

		check_for_required_character(*current_character, CLOSEBRACE, MAX_CLOSE_BRACE, syntax_check_list);

		if (syntax_check_list[(size_t)OPENBRACE] && !syntax_check_list[(size_t)COMMA] && isalpha(*current_character))
		{
			legal.opcode = get_legal_opcode_or_oparand(&current_character, syntax_check_list, LEGALOPCODE);
		}

		if (syntax_check_list[(size_t)OPENBRACE] && syntax_check_list[(size_t)COMMA] && isalnum(*current_character))
		{
			legal.operand = get_legal_opcode_or_oparand(&current_character, syntax_check_list, LEGALOPERAND);
		}

		current_character++;
	}

	Program_Step_Node* next_node = NULL;
	if (print_syntax_errors(syntax_check_list, line_number, file_name, text_line))
	{
		next_node = create_program_step(&legal);
		(*line_number)++;
	}

	return next_node;
}

/*
 * Syntax cecking ends here.
 */

void delete_linked_list_of_program_steps(Program_Step_Node* linked_list_of_program_steps)
{
	Program_Step_Node* current_head_of_list = linked_list_of_program_steps;

	while (current_head_of_list)
	{
		Program_Step_Node* next = current_head_of_list->next_step;
		free(current_head_of_list);
		current_head_of_list = next;
	}
}

#ifdef UNIT_TESTING
/*
 * The unit tests need to be able to access static functions and variables in this file.
 * So include the tests rather than compile as a separate unit.
*/
#include "unit_test_human_readable_program_format.c"
#endif	// UNIT_TESTING
