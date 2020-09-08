/*
 * file_io_vm.c
 *
 * Handles the input and output of text files that contain virtual machine programs.
 */

#ifndef FILE_IO_VM_C
#define FILE_IO_VM_C

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ERH_error_reporting.h"
#include "file_io_vm.h"
#include "HRF_human_readable_program_format.h"
#include "parser.h"
#ifdef UNIT_TESTING
#include "unit_test_logging.h"
#endif

bool write_program_to_file(const HRF_Human_Readable_Program_Format* program, const size_t program_size, FILE* out_file)
{
	bool successful = true;

	if (!out_file)
	{
		ERH_va_report_error_fprintf("File not open\n");
		successful = false;
	}
	else
	{
		fprintf(out_file, "{\n");

		for (size_t step_counter = 0; step_counter < program_size; step_counter++)
		{
			char* step_format = (step_counter == program_size - 1) ? "\t{%s, %ox%x}" : "\t{%s, %ox%x},";
			fprintf(out_file, step_format, OPC_translate_opcode_to_string(program->opcode), (unsigned)program->operand);
		}

		fprintf(out_file, "}\n");

		fflush(out_file);
	}

	return successful;
}

HRF_Human_Readable_Program_Format* read_program_from_file(FILE* input_file, size_t* program_size, const char* file_name)
{
	HRF_Human_Readable_Program_Format* program_image = NULL;
	if (!input_file)
	{
		ERH_va_report_error_fprintf("File not open\n");
	}
	else
	{
		HRF_Program_Step_Node* program_linked_list = NULL;
		register HRF_Program_Step_Node* list_tail = NULL;
		const size_t MINIMUM_LINE_LENGTH = strlen("{");
		size_t prog_size = 0;
		unsigned errors = 0;
		/*
		 * First create a linked list of program steps from the test of the file,
		 * then create a program image from the linked list.
		 * While there are lines in the file read each line and extract the opcode and operand and create a program node with them.
		 */
		unsigned char input_buffer[BUFSIZ];

		size_t line_length = strlen(fgets((char*)input_buffer, BUFSIZ, input_file));
		while (line_length != 0 && strcmp("};", (char*)input_buffer))	// "};" will be the end of the input file
		{
			if (line_length > MINIMUM_LINE_LENGTH)	// 
			{
				if (!program_linked_list)
				{
					list_tail = parser(input_buffer, &prog_size, file_name);
					if (!list_tail)
					{
						errors++;
					}
					program_linked_list = list_tail;
				}
				else
				{
					if (!list_tail)
					{
						errors++;
					}
					else
					{
						list_tail->next_step = parser(input_buffer, &prog_size, file_name);
						list_tail = list_tail->next_step;
					}
				}
			}
			else
			{
				errors++;
			}
			line_length = strlen(fgets((char*)input_buffer, BUFSIZ, input_file));
		}

		if (errors)
		{
			ERH_va_report_error_fprintf("check_line_syntax_return_program_step_if_valid encountered errors in read_program_from_file()\n. No program generated\n");
			HRF_delete_linked_list_of_program_steps(program_linked_list);		// no memory leaks
			program_linked_list = NULL;
		}

		if (program_linked_list)
		{
			program_image = HRF_convert_link_list_program_to_array(program_linked_list, prog_size);
			*program_size = prog_size;
			HRF_delete_linked_list_of_program_steps(program_linked_list);		// no memory leaks
		}
	}

	return program_image;
}

#endif // !FILE_IO_VM_C
