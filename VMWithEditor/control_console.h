#ifndef CONTROL_CONSOLE_H
#define CONTROL_CONSOLE_H

#include <stdbool.h>
#include <stdint.h>
#include "arg_flags.h"
#include "human_readable_program_format.h"

bool initialize_control_console(ARGF_ARG_FLAGS_PTR command_line_arguments);
bool console_edit_and_run_program_file(const char* file_name);
bool save_program_to_file(const Human_Readable_Program_Format* program, const size_t program_size, const char* file_name);

#endif
