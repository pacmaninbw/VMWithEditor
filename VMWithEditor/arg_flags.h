/*
 * arg_flags.h
 */
#ifndef ARG_FLAGS_H
#define ARG_FLAGS_H

#include <stdint.h>
#include <stdbool.h>

typedef struct arg_flags
{
	const char** input_files;
	const size_t input_files_count;
	bool use_console_text;
	bool use_graphic_interface;
	bool run_input_file;
	bool edit_input_file;
	bool use_default_program;
} ARG_FLAGS, *ARG_FLAGS_PTR;

ARG_FLAGS_PTR construct_arg_flags_from_argc_argv(const int argc, const char** argv);
ARG_FLAGS_PTR delete_arguments(ARG_FLAGS_PTR arguments);

#endif // !ARG_FLAGS_H
