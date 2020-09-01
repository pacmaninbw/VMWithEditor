#ifndef TEXT_EDITOR_CONSOLE_H
#define TEXT_EDITOR_CONSOLE_H

#include "opcode.h"
#include "human_readable_program_format.h"
#include "arg_flags.h"

bool initialize_text_editor(ARG_FLAGS_PTR command_line_arguments);
bool save_program_to_file(Human_Readable_Program_Format* program, size_t program_size, const char* file_name);

#endif
