#ifndef CONTROL_CONSOLE_H
#define CONTROL_CONSOLE_H

#include "human_readable_program_format.h"
#include "arg_flags.h"
#include <stdbool.h>

bool initialize_control_console(ARG_FLAGS_PTR command_line_arguments);
bool console_edit_and_run_program_file(const char* file_name);

#endif
