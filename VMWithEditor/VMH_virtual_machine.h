/*
 * VMH_virtual_machine.h
 *
 * The public interfaces to the virtual machine.
 */

#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <stdint.h>
#include <stdbool.h>
#include "OPC_opcode.h"
#include "HRF_human_readable_program_format.h"

bool VMH_initialize_virtual_machine(void);
void VMH_reset_virtual_machine(void);
void VMH_run_virtual_machine(void);
bool VMH_load_and_run_program(const HRF_Human_Readable_Program_Format program[],
	const size_t program_size);
bool VMH_program_fits_in_memory(const size_t program_size);
size_t VMH_get_maximum_operand_value(void);
bool VMH_is_legal_operand(const unsigned operand);
long VMH_translate_text_to_operand_and_validate(const char* string_operand);

#endif	// !VIRTUAL_MACHINE_H
