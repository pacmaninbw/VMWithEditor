#ifndef COMMON_PROGRAM_LOGIC_H
#define COMMON_PROGRAM_LOGIC_H
#include "arg_flags.h"
#include "human_readable_program_format.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

extern FILE* error_out_file;

#ifndef strdup
#ifdef _MSC_VER
#if _MSC_VER > 1920
#define strdup _strdup
#endif
#else
#define strdup mystrdup      
#endif
#endif

char* mystrdup(const char* string_to_copy);
unsigned char* ucstrdup(const unsigned char* string_to_copy); 
extern bool init_vm_error_reporting(const char* error_log_file_name);
extern bool execute_original_program_logic(ARG_FLAGS_PTR command_line_args);
extern Human_Readable_Program_Format* default_program(size_t* program_size);
extern bool set_up_command_line_args_and_vm(ARG_FLAGS_PTR* command_line_args, int argc, char* argv[]);
extern void disengage_error_reporting(void);
extern void report_error_generic(char* error_message);

#endif // !COMMON_PROGRAM_LOGIC_H

