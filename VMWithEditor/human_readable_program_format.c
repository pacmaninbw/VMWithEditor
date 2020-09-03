/*
 * human_readable_program_format.c
 * Contains the implementation of the Human Readable parser and support code.
 * If generic linked lists were necessary they would be implemented in their own file.
 * The lower level Unit Tests are include because static functions need to be tested.
 *
 * Provides the API for using the Human Readable Programing Format (assembly language for the virtual machine).
 * A program consists of assembly language instructions of the format '{' OPCODE ',' OPERAND '}' [','].
 * Unfortunately due to the simple nature of the parser all instructions require an OPCODE OPERAND pair.
 * A program can be either a linked list of Program_Step_Nodes or an array of Human_Readable_Program_Format.
 * The ability to convert from aaray to linked list and back again is provided.
 *
 */

#include "error_reporting.h"
#include "human_readable_program_format.h"
#include "lexical_analyzer.h"
#include "virtual_machine.h"
#ifdef UNIT_TESTING
#include "common_unit_test_logic.h"
#endif
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static bool conversion_function_has_required_parameters(void *program, size_t program_size, char * error_function_name)
{
	bool has_required_parameters = true;

	if (!program || !program_size)
	{
		if (!program)
		{
			fprintf(error_out_file, "In %s, linked_program is NULL\n", error_function_name);
		}

		if (!program_size)
		{
			fprintf(error_out_file, "In %s, program_size is zero.\n", error_function_name);
		}
		has_required_parameters = false;
	}

	return has_required_parameters;
}

Human_Readable_Program_Format* convert_link_list_program_to_array(Program_Step_Node *linked_program, size_t program_size)
{
	if (!conversion_function_has_required_parameters((void *)linked_program, program_size, "convert_link_list_program_to_array(Program_Step_Node *linked_program, size_t program_size)"))
	{
		return NULL;
	}

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

Program_Step_Node* convert_array_program_to_linked_list(Human_Readable_Program_Format* array_program, size_t program_size)
{
	if (!conversion_function_has_required_parameters((void*)array_program, program_size, "convert_array_program_to_linked_list(Human_Readable_Program_Format* array_program, size_t program_size)"))
	{
		return NULL;
	}

	Program_Step_Node* head = NULL;
	Program_Step_Node* tail = NULL;
	bool memory_allocation_failed = false;

	for (size_t program_step = 0; program_step < program_size && !memory_allocation_failed; program_step++)
	{
		if (!head)
		{
			tail = create_program_step(&array_program[program_step]);
			head = tail;
			if (!tail)
			{
				memory_allocation_failed = true;
			}
		}
		else
		{
			tail->next_step = create_program_step(&array_program[program_step]);
			if (!tail->next_step)
			{
				memory_allocation_failed = true;
			}
		}
		tail = (tail) ? tail->next_step : NULL;
	}

	if (memory_allocation_failed)
	{
		delete_linked_list_of_program_steps(head);
		head = NULL;
	}

	return head;
}


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
#ifdef HRF_UNIT_TESTING
#include "unit_test_human_readable_program_format.c"
#endif	// HRF_UNIT_TESTiNG
#endif	// UNIT_TESTING
