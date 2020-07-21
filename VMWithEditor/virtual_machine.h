#ifndef VM_H_
#define VM_H_

#include "opcode.h"
#include "human_readable_program_format.h"
#include <stdint.h>
#include <stdbool.h>

extern bool initialize_virtual_machine(void);
extern void reset_vm(void);
extern void run_vm(void);
extern bool load_and_run_program(Human_Readable_Program_Format* program, size_t program_size);
extern bool program_fits_in_memory(size_t program_size);
extern size_t get_maximum_operand_value(void);
extern bool is_legal_operand(unsigned operand);
extern long translate_text_to_operand_and_validate(char* string_operand);

#endif
