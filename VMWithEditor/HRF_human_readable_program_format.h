/* 
 * HRF_human_readable_program_format.h
 *
 * Provides the API for using the Human Readable Programing Format (assembly language for the virtual machine).
 * A program consists of assembly language instructions of the format '{' OPCODE ',' OPERAND '}' [','].
 * Unfortunately due to the simple nature of the parser all instructions require an OPCODE OPERAND pair.
 * A program can be either a linked list of Program_Step_Nodes or an array of Human_Readable_Program_Format.
 * The ability to convert from aaray to linked list and back again is provided.
 */

#ifndef HRF_HUMAN_READABLE_PROGRAM_FORMAT_H
#define HRF_HUMAN_READABLE_PROGRAM_FORMAT_H

#include "OPC_opcode.h"
#include <stdint.h>

typedef uint32_t HRF_OperandType;

typedef struct human_readable_program_format {
	OPC_OPCODE opcode;
	HRF_OperandType operand;
} HRF_Human_Readable_Program_Format;

typedef struct program_step_node
{
	HRF_Human_Readable_Program_Format opcode_and_operand;
	struct program_step_node* next_step;
} HRF_Program_Step_Node;

HRF_Human_Readable_Program_Format* HRF_duplicate_program(const HRF_Human_Readable_Program_Format* program, const size_t program_size);
HRF_Program_Step_Node* HRF_create_program_step(const HRF_Human_Readable_Program_Format* user_input);
HRF_Human_Readable_Program_Format* HRF_convert_link_list_program_to_array(const HRF_Program_Step_Node* linked_program, const size_t program_size);
HRF_Program_Step_Node* HRF_convert_array_program_to_linked_list(const HRF_Human_Readable_Program_Format* array_program, const size_t program_size);
void HRF_delete_linked_list_of_program_steps(HRF_Program_Step_Node* linked_list_of_program_steps);

#ifdef UNIT_TESTING
#ifdef HRF_UNIT_TESTING
// Don't need hrf unit tests for most other unit tests.
#include "unit_test_HRF_human_readable_program_format.h"
#endif	// HRF_UNIT_TESTiNG
#endif	// UNIT_TESTING

#endif	// !HRF_HUMAN_READABLE_PROGRAM_FORMAT_H
