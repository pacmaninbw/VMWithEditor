/*
 * ARGF_arg_flags.h
 * Processes the command line arguments. The arg_flags struct contains all
 * information provided by the command line arguments.
 *
 * All global symbols provided by this module are prefaced by ARGF.
 */
#ifndef ARGF_ARG_FLAGS_H
#define ARGF_ARG_FLAGS_H

#include <stdint.h>
#include <stdbool.h>

typedef struct arg_flags
{
	const char** input_files;
	const size_t input_files_count;
	const char* error_reporting_file_name;
	const char* log_file_name;
	bool use_console_text;
	bool use_graphic_interface;
	bool execute_input_file;
	bool edit_input_file;
	bool use_default_program;
} ARGF_ARG_FLAGS, *ARGF_ARG_FLAGS_PTR;

ARGF_ARG_FLAGS_PTR ARGF_construct_arg_flags_from_argc_argv(const int argc, const char** argv);
ARGF_ARG_FLAGS_PTR ARGF_delete_arguments(ARGF_ARG_FLAGS_PTR arguments);

#endif // !ARGF_ARG_FLAGS_H
