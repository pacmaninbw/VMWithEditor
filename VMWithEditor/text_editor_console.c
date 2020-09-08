/*
 * text_editor_console.c
 *
 * This file contains the user interface functions for the console text editor
 * the allows the user to create and edit virtual machine programs.
 */
#ifndef TEXT_EDITOR_CONSOLE_C
#define TEXT_EDITOR_CONSOLE_C

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_console_functions.h"
#include "error_reporting.h"
#include "text_editor_console.h"
#ifdef UNIT_TESTING
#include "unit_test_logging.h"
#endif
#include "virtual_machine.h"


static unsigned get_user_operand_value(size_t operand_minimum, size_t operand_maximum)
{
	unsigned operand = 0;

	return operand;
}

static void show_opcode_menu(void)
{
	for (size_t opcode_index = 0; opcode_index <= (size_t)LASTOPCODE; opcode_index++)
	{
		printf("%d. %s\n", (int)opcode_index, opcode_translator[opcode_index].opcode_string_translation);
	}
}

static void show_edit_menu(void)
{

}

static Program_Step_Node* add_program_step(void)
{
	Human_Readable_Program_Format new_program_step = {HALT, 0};

	printf("To select an opcode for the current step enter the number of the opcode\n");
	show_opcode_menu();
	new_program_step.opcode = get_user_selection((size_t)FIRST_OPCODE, (size_t)LASTOPCODE);

	Program_Step_Node* next_step = calloc(1, sizeof(*next_step));
	if (!next_step)
	{
		ERH_va_report_error_fprintf("In add_program_step(), memory allocation for next_step failed\n");
	}
	else
	{
		next_step->opcode_and_operand.opcode = new_program_step.opcode;
		next_step->opcode_and_operand.operand = new_program_step.operand;
	}

	return next_step;
}

bool initialize_text_editor(const ARG_FLAGS_PTR command_line_arguments)
{
	bool successful = true;

	return successful;
}

#endif	// !TEXT_EDITOR_CONSOLE_C
