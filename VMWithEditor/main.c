/*
 * main.c
 */

#ifndef VIRTUAL_MACHINE_WITH_EDITOR_MAIN_C
#define VIRTUAL_MACHINE_WITH_EDITOR_MAIN_C

#include <stdio.h>
#include <stdlib.h>

#include "control_console.h"
#include "default_program.h"
#include "error_reporting.h"
#include "virtual_machine.h"

static bool execute_original_program_logic(ARGF_ARG_FLAGS_PTR command_line_args)
{
	bool successful = true;

	size_t program_size = 0;
	Human_Readable_Program_Format* program = default_program(&program_size);
	if (program)
	{
		if (!load_and_run_program(program, program_size))
		{
			ERH_va_report_error_fprintf("In execute_program_logic() load_and_run_program() failed\n");
			successful = false;
		}
		free(program);
	}
	else
	{
		successful = false;
	}

	return successful;
}

static ARGF_ARG_FLAGS_PTR  set_up_command_line_args_and_vm(int argc, char* argv[])
{
	ARGF_ARG_FLAGS_PTR command_line_args = ARGF_construct_arg_flags_from_argc_argv(argc, argv);
	if (!command_line_args)
	{
		ERH_va_report_error_fprintf("Due to memory allocation error %s is exiting.\n", argv[0]);
	}

	if (command_line_args)
	{
		if (!initialize_virtual_machine())
		{
			command_line_args = ARGF_delete_arguments(command_line_args);
			ERH_va_report_error_fprintf("Unable to initialize Virtual Machine, %s is exiting\n", argv[0]);
		}
	}

	return command_line_args;
}

static bool execute_program_logic(ARGF_ARG_FLAGS_PTR command_line_args)
{
	bool successful = true;

	if (command_line_args->execute_input_file)
	{
		if (command_line_args->use_default_program)
		{
			successful = execute_original_program_logic(command_line_args);
		}
	}

	return successful;
}

int main(int argc, char *argv[]) {
	ERH_error_out_file = stderr;
	int exit_status = EXIT_SUCCESS;
	ARGF_ARG_FLAGS_PTR command_line_args = set_up_command_line_args_and_vm(argc, argv);

	if (command_line_args)
	{
		if (!execute_program_logic(command_line_args))
		{
			exit_status = EXIT_FAILURE;
		}

		ARGF_delete_arguments(command_line_args);
	}
	else
	{
		exit_status = EXIT_FAILURE;
	}

	return exit_status;
}

#endif // !VIRTUAL_MACHINE_WITH_EDITOR_MAIN_C
