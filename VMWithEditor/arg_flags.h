#ifndef ARG_FLAGS_H
#define ARG_FLAGS_H

#include <stdint.h>
#include <stdbool.h>

typedef struct arg_flags
{
	char** input_files;
	size_t input_files_count;
	bool use_console_text;
	bool use_graphic_interface;
	bool run_input_file;
	bool edit_input_file;
	bool use_default_program;
} ARG_FLAGS, *ARG_FLAGS_PTR;

extern ARG_FLAGS_PTR parse_argc_argv(int argc, char** argv);
extern ARG_FLAGS_PTR delete_arguments(ARG_FLAGS_PTR arguments);

#endif // !ARG_FLAGS_H
