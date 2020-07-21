#ifndef COMMON_PROGRAM_LOGIC_H
#define COMMON_PROGRAM_LOGIC_H
#include <stdio.h>
#include <stdbool.h>
#include "arg_flags.h"
#include "human_readable_program_format.h"

extern FILE* error_out_file;

extern bool init_vm_error_reporting(char* error_log_file_name);
extern bool execute_original_program_logic(ARG_FLAGS_PTR command_line_args);
extern Human_Readable_Program_Format* default_program(size_t* program_size);
extern bool set_up_command_line_args_and_vm(ARG_FLAGS_PTR* command_line_args, int argc, char* argv[]);
extern void disengage_error_reporting(void);

#endif // !COMMON_PROGRAM_LOGIC_H

