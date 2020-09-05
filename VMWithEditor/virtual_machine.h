/*
 * virtual_machine.h
 *
 * The public interfaces to the virtual machine.
 */

#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <stdint.h>
#include <stdbool.h>
#include "opcode.h"
#include "human_readable_program_format.h"

bool initialize_virtual_machine(void);
void reset_vm(void);
void run_vm(void);
bool load_and_run_program(const Human_Readable_Program_Format program[],
	const size_t program_size);
bool program_fits_in_memory(const size_t program_size);
size_t get_maximum_operand_value(void);
bool is_legal_operand(const unsigned operand);
long translate_text_to_operand_and_validate(const char* string_operand);

#endif	// !VIRTUAL_MACHINE_H
