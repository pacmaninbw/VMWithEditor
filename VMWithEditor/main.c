#include "common_program_logic.h"
#include "control_console.h"
#include <stdio.h>
#include <stdlib.h>

static bool execute_program_logic(ARG_FLAGS_PTR command_line_args)
{
	bool successful = true;

	if (command_line_args->use_default_program)
	{
		successful = execute_original_program_logic(command_line_args);
	}

	return successful;
}

int main(int argc, char *argv[]) {
	error_out_file = stderr;
	int exit_status = EXIT_SUCCESS;
	ARG_FLAGS_PTR command_line_args = NULL;

	if (set_up_command_line_args_and_vm(&command_line_args, argc, argv))
	{
		if (!execute_program_logic(command_line_args))
		{
			exit_status = EXIT_FAILURE;
		}

		delete_arguments(command_line_args);
	}
	else
	{
		exit_status = EXIT_FAILURE;
	}

	return exit_status;
}