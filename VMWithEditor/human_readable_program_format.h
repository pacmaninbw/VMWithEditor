#ifndef HUMAN_READABLE_PROGRAM_FORMAT_H
#define HUMAN_READABLE_PROGRAM_FORMAT_H

#include "opcode.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

typedef uint32_t OperandType;

typedef struct human_readable_program_format {
	OPCODE opcode;
	OperandType operand;
} Human_Readable_Program_Format;

typedef struct program_step_node
{
	Human_Readable_Program_Format opcode_and_operand;
	struct program_step_node* next_step;
} Program_Step_Node;

extern Human_Readable_Program_Format* duplicate_program(Human_Readable_Program_Format* program, size_t program_size);
extern Program_Step_Node* create_program_step(Human_Readable_Program_Format* user_input);
extern Human_Readable_Program_Format* convert_link_list_program_to_array(Program_Step_Node* linked_program, size_t program_size);
extern bool write_program_to_file(Human_Readable_Program_Format* program, size_t program_size, FILE* out_file);
extern void delete_linked_list_of_program_steps(Program_Step_Node* linked_list_of_program_steps);
extern Human_Readable_Program_Format* read_program_from_file(FILE* input_file, size_t* program_size, char* file_name);
extern Program_Step_Node* hrf_check_line_syntax_return_program_step_if_valid(unsigned char* text_line, size_t* line_number, char* file_name);

#ifdef UNIT_TESTING
#include "unit_test_human_readable_program_format.h"
#endif	// UNIT_TESTING

#endif
